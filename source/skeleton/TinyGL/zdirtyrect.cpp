/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/*
 * This file is based on, or a modified version of code from TinyGL (C) 1997-1998 Fabrice Bellard,
 * which is licensed under the zlib-license (see LICENSE).
 * It also has modifications by the ResidualVM-team, which are covered under the GPLv2 (or later).
 */

#include "zdirtyrect.h"
#include "zgl.h"
#include "gl.h"
//#include "common/debug.h"
#include "tglmath.h"

namespace TinyGL {

void tglIssueDrawCall(Graphics::DrawCall *drawCall) {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	c->_drawCallsQueue.push_back(drawCall);
}

void tglDrawRectangle(Common::Rect rect, int r, int g, int b) {
	TinyGL::GLContext *c = TinyGL::gl_get_context();

	if (rect.left < 0)
		rect.left = 0;
	if (rect.right >= c->fb->xsize)		
		rect.right = c->fb->xsize - 1;
	if (rect.top < 0)
		rect.top = 0;
	if (rect.bottom >= c->fb->ysize) 
		rect.bottom = c->fb->ysize - 1;

	for(int x = rect.left; x < rect.right; x++) {
		c->fb->writePixel(rect.top * c->fb->xsize + x, 255, r, g, b);
		c->fb->writePixel(rect.bottom * c->fb->xsize + x, 255, r, g, b);
	}
	for(int y = rect.top; y < rect.bottom; y++) {
		c->fb->writePixel(y * c->fb->xsize + rect.left, 255, r, g, b);
		c->fb->writePixel(y * c->fb->xsize + rect.right, 255, r, g, b);
	}
}

struct DirtyRectangle {
	Common::Rect rectangle;
	int r, g, b;

	DirtyRectangle() { }
	DirtyRectangle(Common::Rect rect, int red, int green, int blue) {
		this->rectangle = rect;
		this->r = red;
		this->g = green;
		this->b = blue;
	}
};


void tglDisposeResources(TinyGL::GLContext *c) {
	// Dispose textures and resources.
	bool allDisposed = true;
	do {
		allDisposed = true;
		TinyGL::GLTexture *t = c->shared_state.texture_hash_table[0];
		while (t) {
			if (t->disposed) {
				TinyGL::free_texture(c, t->handle);
				allDisposed = false;
				break;
			}
			t = t->next;
		}

	} while (allDisposed == false);

	Graphics::Internal::tglCleanupImages();
}

void tglPresentBufferDirtyRects(TinyGL::GLContext *c) {
	typedef Common::List<Graphics::DrawCall *>::const_iterator DrawCallIterator;
	typedef Common::List<TinyGL::DirtyRectangle>::iterator RectangleIterator;

	Common::List<DirtyRectangle> rectangles;

	DrawCallIterator itFrame = c->_drawCallsQueue.begin();
	DrawCallIterator endPrevFrame = c->_previousFrameDrawCallsQueue.end();

	// Compare draw calls.
	if (c->_drawCallsQueue.size() > 0) {
		for (DrawCallIterator itPrevFrame = c->_previousFrameDrawCallsQueue.begin();
			itPrevFrame != endPrevFrame;
			++itPrevFrame, ++itFrame) {
				const Graphics::DrawCall &currentCall = **itFrame;
				const Graphics::DrawCall &previousCall = **itPrevFrame;

				if (previousCall != currentCall) {
					while (itPrevFrame != endPrevFrame) {
						Graphics::DrawCall *dirtyDrawCall = *itPrevFrame;
						rectangles.push_back(DirtyRectangle(dirtyDrawCall->getDirtyRegion(), 255, 255, 255));
						++itPrevFrame;
					}
					break;
				}
		}
	}

	for ( ; itFrame != c->_drawCallsQueue.end(); ++itFrame) {
		const Graphics::DrawCall &currentCall = **itFrame;
		rectangles.push_back(DirtyRectangle(currentCall.getDirtyRegion(), 255, 0, 0));
	}

	// This loop increases outer rectangle coordinates to favor merging of adjacent rectangles.
	for (RectangleIterator it = rectangles.begin(); it != rectangles.end(); ++it) {
		(*it).rectangle.right++;
		(*it).rectangle.bottom++;
	}

	// Merge coalesce dirty rects.
	bool restartMerge;
	do {
		restartMerge = false;
		for (RectangleIterator it1 = rectangles.begin(); it1 != rectangles.end(); ++it1) {
			for (RectangleIterator it2 = rectangles.begin(); it2 != rectangles.end();) {
				if (it1 != it2) {
					if ((*it1).rectangle.intersects((*it2).rectangle)) {
						(*it1).rectangle.extend((*it2).rectangle);
						it2 = rectangles.erase(it2);
						restartMerge = true;
					} else {
						++it2;
					}
				} else {
					++it2;
				}
			}
		}
	} while(restartMerge);

	Common::Rect renderRect(0, 0, c->fb->xsize - 1, c->fb->ysize - 1);

	for (RectangleIterator it1 = rectangles.begin(); it1 != rectangles.end(); ++it1) {
		RectangleIterator it2 = it1;
		it2++;
		while (it2 != rectangles.end()) {
			if ((*it1).rectangle.contains((*it2).rectangle)) {
				it2 = rectangles.erase(it2);
			} else {
				++it2;
			}
		}
		(*it1).rectangle.clip(renderRect);
	}

	// Execute draw calls.
	for (DrawCallIterator it = c->_drawCallsQueue.begin(); it != c->_drawCallsQueue.end(); ++it) {
		Common::Rect drawCallRegion = (*it)->getDirtyRegion();
		for (RectangleIterator itRect = rectangles.begin(); itRect != rectangles.end(); ++itRect) {
			Common::Rect dirtyRegion = (*itRect).rectangle;
			if (dirtyRegion.intersects(drawCallRegion) || drawCallRegion.contains(dirtyRegion)) {
				(*it)->execute(dirtyRegion, true);
			}
		}
	}

	// Dispose not necessary draw calls.
	for (DrawCallIterator it = c->_previousFrameDrawCallsQueue.begin(); it != c->_previousFrameDrawCallsQueue.end(); ++it) {
		delete *it;
	}

	c->_previousFrameDrawCallsQueue = c->_drawCallsQueue;
	c->_drawCallsQueue.clear();

#if TGL_DIRTY_RECT_SHOW
	// Draw debug rectangles.
	// Note: white rectangles are rectangle that contained other rectangles
	// blue rectangles are rectangle merged from other rectangles
	// red rectangles are original dirty rects

	bool blendingEnabled = c->fb->isBlendingEnabled();
	bool alphaTestEnabled = c->fb->isAplhaTestEnabled();
	c->fb->enableBlending(false);
	c->fb->enableAlphaTest(false);

	for (RectangleIterator it = rectangles.begin(); it != rectangles.end(); ++it) {
		tglDrawRectangle((*it).rectangle, (*it).r, (*it).g, (*it).b);
	}

	c->fb->enableBlending(blendingEnabled);
	c->fb->enableAlphaTest(alphaTestEnabled);
#endif

	tglDisposeResources(c);

	c->_currentAllocatorIndex++;
	if (c->_currentAllocatorIndex == 2) {
		c->_currentAllocatorIndex = 0;
		c->_drawCallAllocator[0].reset();
		c->_drawCallAllocator[1].reset();
	}
}

void tglPresentBufferSimple(TinyGL::GLContext *c) {
	typedef Common::List<Graphics::DrawCall *>::const_iterator DrawCallIterator;

	for (DrawCallIterator it = c->_drawCallsQueue.begin(); it != c->_drawCallsQueue.end(); ++it) {
		(*it)->execute(true);
		delete *it;
	}

	c->_drawCallsQueue.clear();

	tglDisposeResources(c);

	c->_drawCallAllocator[c->_currentAllocatorIndex].reset();
}

void tglPresentBuffer() {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	if (c->_enableDirtyRectangles) {
		tglPresentBufferDirtyRects(c);
	} else {
		tglPresentBufferSimple(c);
	}
}

} // end of namespace TinyGL

namespace Graphics {

bool DrawCall::operator==(const DrawCall &other) const {
	if (_type == other._type) {
		switch (_type) {
		case DrawCall_Rasterization:
			return *(const RasterizationDrawCall *)this == (const RasterizationDrawCall &)other;
			break;
		case DrawCall_Blitting:
			return *(const BlittingDrawCall *)this == (const BlittingDrawCall &)other;
			break;
		case DrawCall_Clear:
			return *(const ClearBufferDrawCall *)this == (const ClearBufferDrawCall &)other;
			break;
		default:
			return false;
		}
	} else {
		return false;
	}
}

RasterizationDrawCall::RasterizationDrawCall() : DrawCall(DrawCall_Rasterization) {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	_vertexCount = c->vertex_cnt;
	_vertex = (TinyGL::GLVertex *) ::Internal::allocateFrame(_vertexCount * sizeof(TinyGL::GLVertex));
	_drawTriangleFront = c->draw_triangle_front;
	_drawTriangleBack = c->draw_triangle_back;
	memcpy(_vertex, c->vertex, sizeof(TinyGL::GLVertex) * _vertexCount);
	_state = captureState();
	computeDirtyRegion();
}

void RasterizationDrawCall::computeDirtyRegion() {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	int width = c->fb->xsize;
	int height = c->fb->ysize;

	int left = width, right = 0, top = height, bottom = 0;

	TinyGL::Vector4 minPc(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	TinyGL::Vector4 maxPc(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN);

	bool pointInsideVolume = false;

	for (int i = 0; i < _vertexCount; i++) {
		TinyGL::GLVertex *v = &_vertex[i];
		float winv;
		// coordinates
		winv = (float)(1.0 / v->pc.W);
		float pcX = v->pc.X * winv;
		float pcY = v->pc.Y * winv;
		int screenCoordsX = (int)(pcX * c->viewport.scale.X + c->viewport.trans.X);
		int screenCoordsY = (int)(pcY * c->viewport.scale.Y + c->viewport.trans.Y);

		left = MIN(left, screenCoordsX);
		right = MAX(right, screenCoordsX);
		top = MIN(top, screenCoordsY);
		bottom = MAX(bottom, screenCoordsY);

		if (!pointInsideVolume) {
			if (pcX >= -2 && pcX <= 2 && pcY >= -2 && pcY <= 2) { // Normalized cube clipping.
				pointInsideVolume = true;
			}
		}
	}

	// Clipping out of screen cases.
	// Reason: other "out of screen cases are actually full screen quads"
	if (pointInsideVolume == false) {
		left = right = top = bottom = 0;
	}

	// Those nested ifs cover the case where the triangle is slightly offscreen
	// but it should still be rendered.

	if (left < 0) {
		left = 0;
		if (right < left) {
			left = 0;
			right = width - 1;
		}
	}

	if (right >= width) {
		right = width - 1;
		if (left > right) {
			left = 0;
			right = width - 1;
		}
	}

	if (top < 0) {
		top = 0;
		if (bottom < top) {
			top = 0;
			bottom = height - 1;
		}
	}

	if (bottom >= height) {
		bottom = height - 1;
		if (top > bottom) {
			top = 0;
			bottom = height - 1;
		}
	}

	_dirtyRegion = Common::Rect(left, top, right, bottom);
	// This takes into account precision issues that occur during rasterization.
	_dirtyRegion.left -= 2;
	_dirtyRegion.top -= 2;
	_dirtyRegion.right += 2;
	_dirtyRegion.bottom += 2;
}

void RasterizationDrawCall::execute(bool restoreState) const {
	TinyGL::GLContext *c = TinyGL::gl_get_context();

	RasterizationDrawCall::RasterizationState backupState;
	if (restoreState) {
		backupState = captureState();
	}
	applyState(_state);

	TinyGL::GLVertex *prevVertex = c->vertex;
	int prevVertexCount = c->vertex_cnt;

	c->vertex = _vertex;
	c->vertex_cnt = _vertexCount;
	c->draw_triangle_front = (TinyGL::gl_draw_triangle_func)_drawTriangleFront;
	c->draw_triangle_back = (TinyGL::gl_draw_triangle_func)_drawTriangleBack;

	int n = c->vertex_n;
	int cnt = c->vertex_cnt;

	switch (c->begin_type) {
	case TGL_POINTS:
		for(int i = 0; i < cnt; i++) {
			gl_draw_point(c, &c->vertex[i]);
		}
		break;
	case TGL_LINES:
		for(int i = 0; i < cnt / 2; i++) {
			gl_draw_line(c, &c->vertex[i * 2], &c->vertex[i * 2 + 1]);
		}
		break;
	case TGL_LINE_STRIP:
	case TGL_LINE_LOOP:
		for(int i = 0; i < cnt; i++) {
			gl_draw_line(c, &c->vertex[i], &c->vertex[i + 1]);
		}
		gl_draw_line(c, &c->vertex[0], &c->vertex[cnt - 1]);
		break;
	case TGL_TRIANGLES:
		for(int i = 0; i < cnt / 3; i++) {
			gl_draw_triangle(c, &c->vertex[i * 3], &c->vertex[i * 3 + 1], &c->vertex[i * 3 + 2]);
		}
		break;
	case TGL_TRIANGLE_STRIP:
		while (cnt >= 3) {
			// needed to respect triangle orientation
			switch (cnt & 1) {
			case 0:
				gl_draw_triangle(c, &c->vertex[2], &c->vertex[1], &c->vertex[0]);
				break;
			case 1:
				gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
				break;
			}
			cnt--;
			c->vertex++;
		}
		break;
	case TGL_TRIANGLE_FAN:
		for(int i = 1; i < cnt; i += 2) {
			gl_draw_triangle(c, &c->vertex[0], &c->vertex[i], &c->vertex[i + 1]);
		}
		break;
	case TGL_QUADS:
		for(int i = 0; i < cnt / 4; i++) {
			c->vertex[i + 2].edge_flag = 0;
			gl_draw_triangle(c, &c->vertex[i], &c->vertex[i + 1], &c->vertex[i + 2]);
			c->vertex[i + 2].edge_flag = 1;
			c->vertex[i + 0].edge_flag = 0;
			gl_draw_triangle(c, &c->vertex[i], &c->vertex[i + 2], &c->vertex[i + 3]);
		}
		break;
	case TGL_QUAD_STRIP:
		for( ; n >= 4; n -= 2) {
			gl_draw_triangle(c, &c->vertex[0], &c->vertex[1], &c->vertex[2]);
			gl_draw_triangle(c, &c->vertex[1], &c->vertex[3], &c->vertex[2]);
			for (int i = 0; i < 2; i++) {
				c->vertex[i] = c->vertex[i + 2];
			}
		}
		break;
	case TGL_POLYGON: {
		for (int i = c->vertex_cnt; i >= 3; i--) {
			gl_draw_triangle(c, &c->vertex[i - 1], &c->vertex[0], &c->vertex[i - 2]);
		}
		break;
	}
	default:
		printf("glBegin: type %x not handled", c->begin_type);
	}

	c->vertex = prevVertex;
	c->vertex_cnt = prevVertexCount;

	if (restoreState) {
		applyState(backupState);
	}
}

RasterizationDrawCall::RasterizationState RasterizationDrawCall::captureState() const {
	RasterizationState state;
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	state.alphaTest = c->fb->isAlphaTestEnabled();
	c->fb->getBlendingFactors(state.sfactor, state.dfactor);
	state.enableBlending = c->fb->isBlendingEnabled();
	state.alphaFunc = c->fb->getAlphaTestFunc();
	state.alphaRefValue = c->fb->getAlphaTestRefVal();

	state.cullFaceEnabled = c->cull_face_enabled;
	state.beginType = c->begin_type;
	state.colorMask = c->color_mask;
	state.currentFrontFace = c->current_front_face;
	state.currentShadeModel = c->current_shade_model;
	state.depthTest = c->depth_test;
	state.polygonModeBack = c->polygon_mode_back;
	state.polygonModeFront = c->polygon_mode_front;
	state.shadowMode = c->shadow_mode;
	state.texture2DEnabled = c->texture_2d_enabled;
	state.texture = c->current_texture;
	state.shadowMaskBuf = c->fb->shadow_mask_buf;
	state.depthFunction = c->fb->getDepthFunc();
	state.depthWrite = c->fb->getDepthWrite();
	state.lightingEnabled = c->lighting_enabled;
	state.depthTestEnabled = c->fb->getDepthTestEnabled();
	if (c->current_texture != nullptr) 
		state.textureVersion = c->current_texture->versionNumber;

	memcpy(state.viewportScaling, c->viewport.scale._v, sizeof(c->viewport.scale._v));
	memcpy(state.viewportTranslation, c->viewport.trans._v, sizeof(c->viewport.trans._v));
	memcpy(state.currentColor, c->longcurrent_color, sizeof(c->longcurrent_color));

	return state;
}

void RasterizationDrawCall::applyState(const RasterizationDrawCall::RasterizationState &state) const {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	c->fb->setBlendingFactors(state.sfactor, state.dfactor);
	c->fb->enableBlending(state.enableBlending);
	c->fb->enableAlphaTest(state.alphaTest);
	c->fb->setAlphaTestFunc(state.alphaFunc, state.alphaRefValue);
	c->fb->setDepthFunc(state.depthFunction);
	c->fb->enableDepthWrite(state.depthWrite);
	c->fb->enableDepthTest(state.depthTestEnabled);

	c->lighting_enabled = state.lightingEnabled;
	c->cull_face_enabled = state.cullFaceEnabled;
	c->begin_type = state.beginType;
	c->color_mask = state.colorMask;
	c->current_front_face = state.currentFrontFace;
	c->current_shade_model = state.currentShadeModel;
	c->depth_test = state.depthTest;
	c->polygon_mode_back = state.polygonModeBack;
	c->polygon_mode_front = state.polygonModeFront;
	c->shadow_mode = state.shadowMode;
	c->texture_2d_enabled = state.texture2DEnabled;
	c->current_texture = state.texture; 
	c->fb->shadow_mask_buf = state.shadowMaskBuf;

	memcpy(c->viewport.scale._v, state.viewportScaling, sizeof(c->viewport.scale._v));
	memcpy(c->viewport.trans._v, state.viewportTranslation, sizeof(c->viewport.trans._v));
	memcpy(c->longcurrent_color, state.currentColor, sizeof(c->longcurrent_color));
}

void RasterizationDrawCall::execute(const Common::Rect &clippingRectangle, bool restoreState) const {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	c->fb->setScissorRectangle(clippingRectangle.left, clippingRectangle.right, clippingRectangle.top, clippingRectangle.bottom);
	execute(restoreState);
	c->fb->setScissorRectangle(0, c->fb->xsize, 0, c->fb->ysize);
}

const Common::Rect RasterizationDrawCall::getDirtyRegion() const {
	return _dirtyRegion;
}

bool RasterizationDrawCall::operator==(const RasterizationDrawCall &other) const {
	if (_vertexCount == other._vertexCount && 
		_drawTriangleFront == other._drawTriangleFront && 
		_drawTriangleBack == other._drawTriangleBack && 
		_state == other._state) {
		for (int i = 0; i < _vertexCount; i++) {
			if ((_vertex[i] != other._vertex[i])) {
				return false;
			}
		}
		return true;
	}
	return false;
}

BlittingDrawCall::BlittingDrawCall(Graphics::BlitImage *image, const BlitTransform &transform, BlittingMode blittingMode) : DrawCall(DrawCall_Blitting), _transform(transform), _mode(blittingMode), _image(image) {
	_blitState = captureState();
	_imageVersion = tglGetBlitImageVersion(image);
}

void BlittingDrawCall::execute(bool restoreState) const {
	BlittingState backupState;
	if (restoreState) {
		backupState = captureState();
	}
	applyState(_blitState);

	switch (_mode) {
	case Graphics::BlittingDrawCall::BlitMode_Regular:
		Graphics::Internal::tglBlit(_image, _transform);
		break;
	case Graphics::BlittingDrawCall::BlitMode_NoBlend:
		Graphics::Internal::tglBlitNoBlend(_image, _transform);
		break;
	case Graphics::BlittingDrawCall::BlitMode_Fast:
		Graphics::Internal::tglBlitFast(_image, _transform._destinationRectangle.left, _transform._destinationRectangle.top);
		break;
	case Graphics::BlittingDrawCall::BlitMode_ZBuffer:
		Graphics::Internal::tglBlitZBuffer(_image, _transform._destinationRectangle.left, _transform._destinationRectangle.top);
		break;
	default:
		break;
	}
	if (restoreState) {
		applyState(backupState);
	}
}

void BlittingDrawCall::execute(const Common::Rect &clippingRectangle, bool restoreState) const {
	Graphics::Internal::tglBlitSetScissorRect(clippingRectangle.left, clippingRectangle.top, clippingRectangle.right, clippingRectangle.bottom);
	execute(restoreState);
	Graphics::Internal::tglBlitSetScissorRect(0, 0, 0, 0);
}

BlittingDrawCall::BlittingState BlittingDrawCall::captureState() const {
	BlittingState state;
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	state.alphaTest = c->fb->isAlphaTestEnabled();
	c->fb->getBlendingFactors(state.sfactor, state.dfactor);
	state.enableBlending = c->fb->isBlendingEnabled();
	state.alphaFunc = c->fb->getAlphaTestFunc();
	state.alphaRefValue = c->fb->getAlphaTestRefVal();
	state.depthTestEnabled = c->fb->getDepthTestEnabled();
	return state;
}

void BlittingDrawCall::applyState(const BlittingState &state) const {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	c->fb->setBlendingFactors(state.sfactor, state.dfactor);
	c->fb->enableBlending(state.enableBlending);
	c->fb->enableAlphaTest(state.alphaTest);
	c->fb->setAlphaTestFunc(state.alphaFunc, state.alphaRefValue);
	c->fb->enableDepthTest(state.depthTestEnabled);
}

const Common::Rect BlittingDrawCall::getDirtyRegion() const {
	int blitWidth = _transform._destinationRectangle.width();
	int blitHeight = _transform._destinationRectangle.height();
	if (blitWidth == 0) {
		if (_transform._sourceRectangle.width() != 0) {
			blitWidth = _transform._sourceRectangle.width();
		} else {
			tglGetBlitImageSize(_image, blitWidth, blitHeight);
		}
	} 
	if (blitHeight == 0) {
		if (_transform._sourceRectangle.height() != 0) {
			blitHeight = _transform._sourceRectangle.height();
		} else {
			tglGetBlitImageSize(_image, blitWidth, blitHeight);
		}
	}
	return Common::Rect(_transform._destinationRectangle.left, _transform._destinationRectangle.top, _transform._destinationRectangle.left + blitWidth, _transform._destinationRectangle.top + blitHeight);
}

bool BlittingDrawCall::operator==(const BlittingDrawCall &other) const {
	return	_mode == other._mode &&
			_image == other._image &&
			_transform == other._transform &&
			_blitState == other._blitState &&
			_imageVersion == tglGetBlitImageVersion(other._image);
}

ClearBufferDrawCall::ClearBufferDrawCall(bool clearZBuffer, int zValue, bool clearColorBuffer, int rValue, int gValue, int bValue) 
	: _clearZBuffer(clearZBuffer), _clearColorBuffer(clearColorBuffer), _zValue(zValue), _rValue(rValue), _gValue(gValue), _bValue(bValue), DrawCall(DrawCall_Clear) {
}

void ClearBufferDrawCall::execute(bool restoreState) const {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	c->fb->clear(_clearZBuffer, _zValue, _clearColorBuffer, _rValue, _gValue, _bValue);
}

void ClearBufferDrawCall::execute(const Common::Rect &clippingRectangle, bool restoreState) const {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	c->fb->clearRegion(clippingRectangle.left, clippingRectangle.top, clippingRectangle.width(), clippingRectangle.height(), _clearZBuffer, _zValue, _clearColorBuffer, _rValue, _gValue, _bValue);
}

const Common::Rect ClearBufferDrawCall::getDirtyRegion() const {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	return Common::Rect(0, 0, c->fb->xsize, c->fb->ysize);
}

bool ClearBufferDrawCall::operator==(const ClearBufferDrawCall &other) const {
	return	_clearZBuffer == other._clearZBuffer &&
			_clearColorBuffer == other._clearColorBuffer &&
			_rValue == other._rValue &&
			_gValue == other._gValue &&
			_bValue == other._bValue &&
			_zValue == other._zValue;
}


bool RasterizationDrawCall::RasterizationState::operator==(const RasterizationState &other) const {
	return	beginType == other.beginType && 
			currentFrontFace == other.currentFrontFace && 
			cullFaceEnabled == other.cullFaceEnabled &&
			colorMask == other.colorMask &&
			depthTest == other.depthTest &&
			depthFunction == other.depthFunction &&
			depthWrite == other.depthWrite &&
			shadowMode == other.shadowMode &&
			texture2DEnabled == other.texture2DEnabled &&
			currentShadeModel == other.currentShadeModel &&
			polygonModeBack == other.polygonModeBack &&
			polygonModeFront == other.polygonModeFront &&
			lightingEnabled == other.lightingEnabled &&
			enableBlending == other.enableBlending &&
			sfactor == other.sfactor &&
			dfactor == other.dfactor &&
			alphaTest == other.alphaTest &&
			alphaFunc == other.alphaFunc &&
			alphaRefValue == other.alphaRefValue &&
			texture == other.texture &&
			shadowMaskBuf == other.shadowMaskBuf &&
			currentColor[0] == other.currentColor[0] &&
			currentColor[1] == other.currentColor[1] &&
			currentColor[2] == other.currentColor[2] &&
			currentColor[3] == other.currentColor[3] &&
			viewportTranslation[0] == other.viewportTranslation[0] &&
			viewportTranslation[1] == other.viewportTranslation[1] &&
			viewportTranslation[2] == other.viewportTranslation[2] &&
			viewportScaling[0] == other.viewportScaling[0] &&
			viewportScaling[1] == other.viewportScaling[1] &&
			viewportScaling[2] == other.viewportScaling[2] &&
			depthTestEnabled == other.depthTestEnabled &&
			textureVersion == texture->versionNumber;
}

} // end of namespace Graphics


void *Internal::allocateFrame(int size) {
	TinyGL::GLContext *c = TinyGL::gl_get_context();
	return c->_drawCallAllocator[c->_currentAllocatorIndex].allocate(size);
}
