@echo off

set SOLUTION_NAME=trade_processor_project
set BOOST_ROOT=c:/boost/boost_1_54_0

set BUILD_TYPE=Debug
if [%1]==[Release] (
	set BUILD_TYPE=Release
)
set BUILD_FOLDER=_build_%BUILD_TYPE%_32

if not exist %BUILD_FOLDER% (
	mkdir %BUILD_FOLDER%
)

cd %BUILD_FOLDER%
cmake -DBOOST_STATIC=ON -DBOOST_STAGE_FOLDER_WITH_ADDRESS_MODEL=ON -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DSOLUTION_NAME=%SOLUTION_NAME%  -G "Visual Studio 9 2008" ../ 
cd ../

echo "%BUILD_FOLDER%/%SOLUTION_NAME%.sln" > _start_msvc.bat
