cd mastd

mobuild -build mastdD.mopro
cd ..\mautil
mobuild -build mautilD.mopro
cd ..\mtxml
mobuild -build mtxmlD.mopro
cd ..\matest
mobuild -build matestD.mopro
cd ..\mafs
mobuild -build mafsD.mopro
cd ..\maui
mobuild -build mauiD.mopro

cd ..
