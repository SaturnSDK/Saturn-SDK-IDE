export NCPU=`nproc`

./build-ide.sh


if [[ "$CREATEINSTALLER" == "YES" ]]; then
	./createinstaller.sh
fi
