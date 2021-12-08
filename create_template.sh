#!/bin/bash

project_name="$1"
project_author="$2"
project_path="$3"

# check parameters
if [ -z "$project_name" ] || [ -z "$project_author" ] || [ -z "$project_path" ]; then
  echo "error: missing arguments: either project_name, project_author or project_path is empty"
  echo "usage: $0 project_name project_path"
  exit 1
fi

if [ -f "$project_path" ] || [ -d "$project_path" ]; then
  echo "error: project_path already exist"
  echo "usage: $0 project_name project_path"
  exit 1
fi

# install linux build deps
sudo apt-get -y install build-essential cmake git sed libsdl2-dev libfreetype6-dev libglm-dev libconfig-dev libphysfs-dev

#
mkdir -p "$project_path"
pushd "$project_path" || exit 1
git init && git submodule add https://github.com/Cpasjuste/libcross2d.git
cp -r libcross2d/templates/simple/. "$project_path"/.
sed -i "s/PROJECT_NAME_TEMPLATE/$project_name/g" CMakeLists.txt
sed -i "s/PROJECT_AUTHOR_TEMPLATE/$project_author/g" CMakeLists.txt
git add --all . && git commit -am "first commit"
popd || exit 1

echo "########################################################"
echo "# \"$project_name\" template successfully created in $project_path"
echo "# To compile your project (for linux), go to the project directory then:"
echo "#"
echo "# mkdir cmake-build-release && cd cmake-build-release"
echo "# cmake -G \"Unix Makefiles\" -DPLATFORM_LINUX=ON -DCMAKE_BUILD_TYPE=Release ../"
echo "# make"
echo "#"
echo "########################################################"
