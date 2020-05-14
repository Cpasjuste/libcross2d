# Tweeny Changelog
- Version 3.1.0:
  - From now on, tweeny will be using a more traditional versioning scheme
  - Remove some extraneous semicolons (@Omegastick)
  - Adds `easing::stepped` and `easing::def` for arithmetic-like values (@ArnCarveris)
  - Fix point progress calculation in multi-duration tweens (#15)
  - Fix deduction of same-type values (#14)
  - Use `auto` to deduce return values of operations inside various easings

- Version 3:
  - Fix point duration calculation in multipoint tweening 
  - Implement `peek(float progress)` and `peek(uint32_t time)` to peek
    at arbitrary points
  - Move examples to [tweeny-demos](http://github.com/mobius3/tweeny-demos) repository
  - Update README and docs

- Version 2:
  - Make non-modifying functions `const` (@Balletie)
  - Add `peek()` tween method to get current tween values (@Balletie)
  - Fix build on MSVC by constexpr-instantiating standard easings
  - Add a CHANGELOG :)

- Version 1:
  - Initial Release
