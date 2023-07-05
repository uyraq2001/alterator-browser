Name: altcenter
Version: 1.0
Release: alt1

Summary: Revised alterator
License: GPLv2+
URL: https://github.com/uyraq2001/alterator-module-browser

BuildPreReq: cmake rpm-macros-cmake
BuildRequires: libqt5-devel ...

Packager: kozyrevid <kozyrevid@basealt.ru>

%description
Alterator operating via D-Bus.

%prep
%setup

%build
export PATH=$PATH:%_qt4dir/bin
%cmake_insource
%make_build # VERBOSE=1

%install
export PATH=$PATH:%_qt4dir/bin
pushd build
%makeinstall_std
popd
%find_lang %name

%files -f %name.lang
# %doc AUTHORS ChangeLog NEWS README THANKS TODO contrib/ manual/
%_bindir/*
# %_man1dir/*

%changelog
* Sat Jan 33 3001 Example Packager <example@altlinux.org> 1.0-alt1
- initial build
