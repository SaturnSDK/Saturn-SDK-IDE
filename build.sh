export NCPU=`nproc`

./bootstrap

./build-ide.sh

if [ $? -ne 0 ]; then
	echo "Failed building Code::Blocks"
	exit 1
fi


if [[ "$CREATEINSTALLER" == "YES" ]]; then
	./createinstaller.sh
fi
