srcPath=
rootPath=

if [ -n $1 ]; then srcPath=$1;fi
if [ -n $2 ]; then rootPath=$2;fi

echo "srcPath=$srcPath"
echo "rootPath=$rootPath"
echo "tar -zxf $srcPath -C $rootPath"
tar -zxf $srcPath -C $rootPath
