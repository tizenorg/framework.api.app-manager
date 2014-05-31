/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#include <tet_api.h>
#include <app_manager.h>

#include <stdbool.h>
#include <dlog.h>

#undef LOG_TAG
#define LOG_TAG "APPLICATION_MANAGER_TC"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_app_manager_foreach_installed_app_p(void);
static void utc_app_manager_foreach_installed_app_n(void);
static void utc_app_manager_get_app_name_p(void);
static void utc_app_manager_get_app_name_n(void);
static void utc_app_manager_get_app_icon_path_p(void);
static void utc_app_manager_get_app_icon_path_n(void);
static void utc_app_manager_get_app_version_p(void);
static void utc_app_manager_get_app_version_n(void);


struct tet_testlist tet_testlist[] = {
	{ utc_app_manager_foreach_installed_app_p, 1 },
	{ utc_app_manager_foreach_installed_app_n, 2 },
	{ utc_app_manager_get_app_name_p, 1},
	{ utc_app_manager_get_app_name_n, 2},
	{ utc_app_manager_get_app_icon_path_p, 1},
	{ utc_app_manager_get_app_icon_path_n, 2},
	{ utc_app_manager_get_app_version_p, 1},
	{ utc_app_manager_get_app_version_n, 2},
	{ NULL, 0 },
};



static void startup(void)
{
	/* start of TC */
}

static void cleanup(void)
{
	/* end of TC */
}

static bool foreach_cb_application (const char *package_name, void *user_data)
{
	return true;
}

static void utc_app_manager_foreach_installed_app_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_foreach_app_installed(foreach_cb_application, NULL);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_is_running_p", "passed");
	}else{
		dts_fail("utc_app_manager_is_running_p", "failed");
	}
}

static void utc_app_manager_foreach_installed_app_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_foreach_app_installed(NULL, NULL);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_is_running_p", "passed");
	}else{
		dts_fail("utc_app_manager_is_running_p", "failed");
	}

}

static void utc_app_manager_get_app_name_p(void)
{
	char *name;
	char *package_name = "com.samsung.clock";
	int ret = APP_MANAGER_ERROR_NONE;
	
	ret =  app_manager_get_app_name(package_name, &name);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_is_running_p", "passed");
	}else{
		dts_fail("utc_app_manager_is_running_p", "failed");
	}

}
static void utc_app_manager_get_app_name_n(void)
{
	char *name;
	int ret = APP_MANAGER_ERROR_NONE;

	ret =  app_manager_get_app_name(NULL, &name);
	
 	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_is_running_p", "passed");
	}else{
		dts_fail("utc_app_manager_is_running_p", "failed");
	}
}
static void utc_app_manager_get_app_icon_path_p(void)
{
	char *icon_path;
	char *package_name = "com.samsung.clock";
	int ret = APP_MANAGER_ERROR_NONE;

	ret =  app_manager_get_app_icon_path(package_name, &icon_path);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_is_running_p", "passed");
	}else{
		dts_fail("utc_app_manager_is_running_p", "failed");
	}
}
static void utc_app_manager_get_app_icon_path_n(void)
{
	char *icon_path;
	int ret = APP_MANAGER_ERROR_NONE;

	ret =  app_manager_get_app_icon_path(NULL, &icon_path);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_is_running_p", "passed");
	}else{
		dts_fail("utc_app_manager_is_running_p", "failed");
	}
}
static void utc_app_manager_get_app_version_p(void)
{
	char *version;
	char *package_name = "com.samsung.clock";
	int ret = APP_MANAGER_ERROR_NONE;

	ret =  app_manager_get_app_version(package_name, &version);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_is_running_p", "passed");
	}else{
		dts_fail("utc_app_manager_is_running_p", "failed");
	}
}
static void utc_app_manager_get_app_version_n(void)
{
	char *version;
	int ret = APP_MANAGER_ERROR_NONE;

	ret =  app_manager_get_app_version(NULL, &version);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_is_running_p", "passed");
	}else{
		dts_fail("utc_app_manager_is_running_p", "failed");
	}
}

