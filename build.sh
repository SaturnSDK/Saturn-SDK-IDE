export NCPU=`nproc`

./bootstrap

./build-ide.sh


if [[ "$CREATEINSTALLER" == "YES" ]]; then
	./createinstaller.sh
fi
