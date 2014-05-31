#sbs-git:slp/api/app-manager capi-appfw-app-manager 0.1.0 76739af2bfbeb46dc9db0cb3e044f880ddcb9440
Name:       capi-appfw-app-manager
Summary:    Application Manager API
Version: 0.1.29
Release:    1
Group:      TO_BE/FILLED_IN
License:    TO BE FILLED IN
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(vconf)
BuildRequires:  pkgconfig(aul)
BuildRequires:  pkgconfig(ail)
BuildRequires:  pkgconfig(pkgmgr)
BuildRequires:  pkgconfig(pkgmgr-info)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(glib-2.0)
Requires(post): /sbin/ldconfig  
Requires(postun): /sbin/ldconfig

%description
The Application Manager API provides functions to get information about running applications.

%package devel
Summary:  Application Manager API (Development)
Group:    TO_BE/FILLED_IN
Requires: %{name} = %{version}-%{release}

%description devel
The Application Manager API provides functions to get information about running applications. (DEV)


%prep
%setup -q


%build
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=/usr -DFULLVER=%{version} -DMAJORVER=${MAJORVER}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%{_libdir}/libcapi-appfw-app-manager.so.*
%{_bindir}/appmgr_tool
%manifest capi-appfw-app-manager.manifest
/usr/share/license/%{name}

%files devel
%{_includedir}/appfw/*.h
%{_libdir}/libcapi-appfw-app-manager.so
%{_libdir}/pkgconfig/*.pc


