#!/bin/sh

#TODO: make nice.
#TODO: usage text.
#NOTE: build.sh [ACTION] [TARGET] [OPTS]
prepend_str()
{
	local prefix=$1
	local str_lst="$2"
	local str_result=""
	for str in $str_lst; do
		str_result=$str_result" "$prefix$str
	done
	echo $str_result
}

build=$1
if [[ $buildname == "" ]]; then
	build="DEBUG"
fi

src_dir=src
src_files="dggt_math.cxx dggt_mem_utils.cxx dggt_allocator.cxx dggt_test.cxx"
src_input=$(prepend_str "../"$src_dir"/" "$src_files")
build_dir=build
compiler="g++"
compiler_opts="-std=c++17 -D"$build
if [[ $build == "DEBUG" ]]; then
	compiler_opts=$compiler_opts" -ggdb"
fi
output_file="dggt"

mkdir "./"$build_dir 2> /dev/null
pushd "./"$build_dir
$compiler $compiler_opts $src_input -o $output_file
popd
