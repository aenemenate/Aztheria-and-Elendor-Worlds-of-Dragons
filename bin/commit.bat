@echo off

pushd "%~dp0"
cd "..\"

git add .
git commit
git push

popd