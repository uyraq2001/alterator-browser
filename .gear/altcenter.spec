Name: altcenter
Version: 0.0.1
Release: alt1

Summary: Revised alterator
License: GPLv2+
Group: Other
URL: https://github.com/uyraq2001/alterator-module-browser

BuildRequires(pre): rpm-macros-cmake
BuildRequires: cmake
BuildRequires: gcc-c++
BuildRequires: qt5-base-devel
BuildRequires: qt5-tools-devel
BuildRequires: qt5-base-common
BuildRequires: boost-devel-headers

Source0: %name-%version.tar

%description
Alterator operating via D-Bus.

%prep
%setup

%build
%cmake
%cmake_build

%install
%cmakeinstall_std

%files
%_bindir/altcenter
%doc README.md INSTALL.md

%changelog
* Wed Jul 5 2023 Kozyrev Yuri <kozyrevid@altlinux.org> 0.0.1-alt1
- initial build
