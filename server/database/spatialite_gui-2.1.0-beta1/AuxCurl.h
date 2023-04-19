/*
/ AuxCurl.h
/ checking and notifying updated versions
/
/ version 2.1, 2018 August 1
/
/ Author: Sandro Furieri a.furieri@lqt.it
/
/ Copyright (C) 2018  Alessandro Furieri
/
/    This program is free software: you can redistribute it and/or modify
/    it under the terms of the GNU General Public License as published by
/    the Free Software Foundation, either version 3 of the License, or
/    (at your option) any later version.
/
/    This program is distributed in the hope that it will be useful,
/    but WITHOUT ANY WARRANTY; without even the implied warranty of
/    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/    GNU General Public License for more details.
/
/    You should have received a copy of the GNU General Public License
/    along with this program.  If not, see <http://www.gnu.org/licenses/>.
/
*/

extern char *GetUpdateVersion();
extern bool DoDownloadUpdatedPackage(const char *download_url,
                                     unsigned char **data, int *data_len);
extern bool DoDownloadUpdatedPackage(const char *download_url,
                                     unsigned char **data, int *data_len);
