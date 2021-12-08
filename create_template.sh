#!/bin/bash

project_name="$1"
project_path="$2"

# check parameters
if [ -z "${project_name}" ] || [ -z "${project_path}" ]; then
  echo "error: missing arguments: either project_name or project_path is empty"
  echo "usage: $0 project_name project_path"
  exit 1
fi

if [ -d "${project_path}" ]; then
  echo "error: project_path already exist"
  echo "usage: $0 project_name project_path"
fi

# install linux build deps
sudo apt-get -y install build-essential cmake git libsdl2-dev libfreetype6-dev libglm-dev libconfig-dev libphysfs-dev

#
mkdir -p "${project_path}"
pushd "${project_path}" || exit 1
git init && git submodule add https://github.com/Cpasjuste/libcross2d.git
cp -r libcross2d/templates/simple/. "${project_path}"/.
popd || exit 1

echo "all done.."
