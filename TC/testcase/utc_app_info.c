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
static void utc_app_info_app_info_clone_p(void);
static void utc_app_info_app_info_clone_n(void);
static void utc_app_info_app_info_create_p(void);
static void utc_app_info_app_info_create_n(void);
static void utc_app_info_app_info_destroy_p(void);
static void utc_app_info_app_info_destroy_n(void);
static void utc_app_info_get_app_id_p(void);
static void utc_app_info_get_app_id_n(void);
static void utc_app_info_get_icon_p(void);
static void utc_app_info_get_icon_n(void);
static void utc_app_info_get_label_p(void);
static void utc_app_info_get_label_n(void);
static void utc_app_info_get_package_p(void);
static void utc_app_info_get_package_n(void);
static void utc_app_info_get_type_p(void);
static void utc_app_info_get_type_n(void);
static void utc_app_info_foreach_metadata_p(void);
static void utc_app_info_foreach_metadata_n(void);
static void utc_app_info_is_equal_p(void);
static void utc_app_info_is_equal_n(void);
static void utc_app_info_is_enabled_p(void);
static void utc_app_info_is_enabled_n(void);
static void utc_app_info_filter_create_p(void);
static void utc_app_info_filter_create_n(void);
static void utc_app_info_filter_destroy_p(void);
static void utc_app_info_filter_destroy_n(void);
static void utc_app_info_filter_add_bool_p(void);
static void utc_app_info_filter_add_bool_n(void);
static void utc_app_info_filter_add_string_p(void);
static void utc_app_info_filter_add_string_n(void);
static void utc_app_info_filter_foreach_appinfo_p(void);
static void utc_app_info_filter_foreach_appinfo_n(void);
static void utc_app_info_metadata_filter_create_p(void);
static void utc_app_info_metadata_filter_create_n(void);
static void utc_app_info_metadata_filter_destroy_p(void);
static void utc_app_info_metadata_filter_destroy_n(void);
static void utc_app_info_metadata_filter_add_p(void);
static void utc_app_info_metadata_filter_add_n(void);
static void utc_app_info_metadata_filter_foreach_p(void);
static void utc_app_info_metadata_filter_foreach_n(void);

struct tet_testlist tet_testlist[] = {
	{ utc_app_info_app_info_clone_p, 1},
	{ utc_app_info_app_info_clone_n, 2},
	{ utc_app_info_app_info_destroy_p, 1},
	{ utc_app_info_app_info_destroy_n, 2},
	{ utc_app_info_app_info_create_p, 1},
	{ utc_app_info_app_info_create_n, 2},
	{ utc_app_info_get_app_id_p, 1},
	{ utc_app_info_get_app_id_n, 2},
	{ utc_app_info_get_icon_p, 1},
	{ utc_app_info_get_icon_n, 2},
	{ utc_app_info_get_label_p, 1},
	{ utc_app_info_get_label_n, 2},
	{ utc_app_info_get_package_p, 1},
	{ utc_app_info_get_package_n, 2},
	{ utc_app_info_get_type_p, 1},
	{ utc_app_info_get_type_n, 2},
	{ utc_app_info_foreach_metadata_p, 1},
	{ utc_app_info_foreach_metadata_n, 2},
	{ utc_app_info_is_equal_p, 1},
	{ utc_app_info_is_equal_n, 2},
	{ utc_app_info_is_enabled_p, 1},
	{ utc_app_info_is_enabled_n, 2},
	{NULL,0},
	};

static void startup(void)
{
}

static void cleanup(void)
{
}

static void utc_app_manager_foreach_running_app_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_foreach_app_running(NULL, NULL);

	if(ret == APP_MANAGER_ERROR_NONE)
	{
		dts_fail("utc_app_manager_foreach_running_app_n", "failed");
	}
	else
	{
		dts_pass("utc_app_manager_foreach_running_app_n", "passed");
	}
}

static bool get_first_app_cb(const char* package_name, void* user_data)
{
    int name_len = strlen(package_name);

    strncpy((char*)user_data, package_name, name_len);

    return false;
}

static int metadata_func(const char *key, const char *value, void *user_data)
{
	return 0;
}

static int filter_cb(app_info_h app_info, void *user_data)
{
	return 0;
}

static void utc_app_info_app_info_clone_p(void)
{
	app_info_h clone = NULL;
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	app_info_h app_info = NULL;

	ret = app_info_create("org.tizen.indicator", &app_info);
	if (ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_clone(&clone, app_info);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_app_info_clone_p", "passed");
		else
			dts_fail("utc_app_info_app_info_clone_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_app_info_clone_p", "failed");
	}
}

static void utc_app_info_app_info_clone_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_info_clone(NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_app_info_clone_n", "passed");
	else
		dts_fail("utc_app_info_app_info_clone_n", "failed");
}

static void utc_app_info_app_info_create_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	app_info_h app_info = NULL;

	ret = app_info_create("org.tizen.indicator", &app_info);
	if (ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_destroy(app_info);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_app_info_create_p", "passed");
		else
			dts_fail("utc_app_info_app_info_create_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_app_info_create_p", "failed");
	}

}

static void utc_app_info_app_info_create_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_info_create(NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_app_info_create_n", "passed");
	else
		dts_fail("utc_app_info_app_info_create_n", "failed");
}

static void utc_app_info_app_info_destroy_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	app_info_h app_info = NULL;

	ret = app_info_create("org.tizen.indicator", &app_info);
	if (ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_destroy(app_info);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_app_info_destroy_p", "passed");
		else
			dts_fail("utc_app_info_app_info_destroy_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_app_info_destroy_p", "failed");
	}

}

static void utc_app_info_app_info_destroy_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_info_destroy(NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_app_info_destroy_n", "passed");
	else
		dts_fail("utc_app_info_app_info_destroy_n", "failed");
}

static void utc_app_info_get_app_id_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	char * app_id = NULL;
	app_info_h app_info = NULL;

	ret = app_info_create("org.tizen.indicator", &app_info);
	if (ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_get_app_id(app_info, &app_id);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_get_app_id_p", "passed");
		else
			dts_fail("utc_app_info_get_app_id_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_get_app_id_p", "failed");
	}

}

static void utc_app_info_get_app_id_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_info_get_app_id(NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_get_app_id_n", "passed");
	else
		dts_fail("utc_app_info_get_app_id_n", "failed");
}

static void utc_app_info_get_icon_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	char * path = NULL;
	app_info_h app_info = NULL;

	ret = app_info_create("org.tizen.indicator", &app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_get_icon(app_info, &path);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_get_icon_p", "passed");

		else
			dts_fail("utc_app_info_get_icon_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_get_icon_p", "failed");
	}

}

static void utc_app_info_get_icon_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_info_get_icon(NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_get_icon_n", "passed");
	else
		dts_fail("utc_app_info_get_icon_n", "failed");
}

static void utc_app_info_get_label_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	app_info_h app_info = NULL;

	char * label = NULL;
	ret = app_info_create("org.tizen.indicator", &app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_get_label(app_info, &label);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_get_label_p", "passed");

		else
			dts_fail("utc_app_info_get_label_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_get_label_p", "failed");
	}
}

static void utc_app_info_get_label_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_info_get_label(NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_get_label_n", "passed");
	else
		dts_fail("utc_app_info_get_label_n", "failed");
}

static void utc_app_info_get_package_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	char * package = NULL;
	app_info_h app_info = NULL;

	ret = app_info_create("org.tizen.indicator", &app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_get_package(app_info, &package);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_get_package_p", "passed");

		else
			dts_fail("utc_app_info_get_package_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_get_package_p", "failed");
	}
}

static void utc_app_info_get_package_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_info_get_package(NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_get_package_n", "passed");
	else
		dts_fail("utc_app_info_get_package_n", "failed");
}

static void utc_app_info_get_type_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	char * type = NULL;
	app_info_h app_info = NULL;

	ret = app_info_create("org.tizen.indicator", &app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_get_package(app_info, &type);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_get_type_p", "passed");

		else
			dts_fail("utc_app_info_get_type_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_get_type_p", "failed");
	}
}

static void utc_app_info_get_type_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_info_get_type(NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_get_type_n", "passed");
	else
		dts_fail("utc_app_info_get_type_n", "failed");
}

static void utc_app_info_foreach_metadata_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_h app_info = NULL;

	ret = app_info_create("org.tizen.indicator", &app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_foreach_metadata(app_info, metadata_func, NULL);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_foreach_metadata_p", "passed");

		else
			dts_fail("utc_app_info_foreach_metadata_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_foreach_metadata_p", "failed");
	}
}

static void utc_app_info_foreach_metadata_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_info_get_type(NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_foreach_metadata_n", "passed");
	else
		dts_fail("utc_app_info_foreach_metadata_n", "failed");
}

static void utc_app_info_is_equal_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_h lhs,rhs;
	bool equal = false;
	ret = app_info_create("org.tizen.indicator", &lhs);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_create("org.tizen.indicator", &rhs);
		if (ret == APP_MANAGER_ERROR_NONE){
			ret = app_info_is_equal(lhs,rhs,&equal);
			if (ret == APP_MANAGER_ERROR_NONE)
				dts_pass("utc_app_info_is_equal_p", "passed");
			else
				dts_fail("utc_app_info_is_equal_p", "failed");
		}
	}
	else
	{
		dts_fail("utc_app_info_get_type_p", "failed");
	}
}

static void utc_app_info_is_equal_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	bool * equal;
	ret = app_info_is_equal(NULL, NULL, equal);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_is_equal_n", "passed");
	else
		dts_fail("utc_app_info_is_equal_n", "failed");
}

static void utc_app_info_is_enabled_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_h app_info;
	bool enabled = false;
	ret = app_info_create("org.tizen.indicator", &app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_create("org.tizen.indicator", &app_info);
		if (ret == APP_MANAGER_ERROR_NONE){
			ret = app_info_is_enabled(app_info,&enabled);
			if (ret == APP_MANAGER_ERROR_NONE)
				dts_pass("utc_app_info_is_enabled_p", "passed");
			else
				dts_fail("utc_app_info_is_enabled_p", "failed");
		}
	}
	else
	{
		dts_fail("utc_app_info_get_type_p", "failed");
	}
}

static void utc_app_info_is_enabled_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	bool enabled;
	app_info_h app_info;

	ret = app_info_is_enabled(app_info, &enabled);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_is_enabled_n", "passed");
	else
		dts_fail("utc_app_info_is_enabled_n", "failed");
}


static void utc_app_info_filter_create_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_filter_h handle = NULL;
	ret = app_info_filter_create(&handle);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_filter_add_string(handle, PACKAGE_INFO_PROP_APP_TYPE, "capp");
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_filter_create_p", "passed");
		else
			dts_fail("utc_app_info_filter_create_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_filter_create_p", "failed");
	}
}

static void utc_app_info_filter_create_n(void)
{
	dts_fail("utc_app_info_filter_create_n", "failed");
}

static void utc_app_info_filter_destroy_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_filter_h handle = NULL;
	ret = app_info_filter_create(&handle);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_filter_add_string(handle, PACKAGE_INFO_PROP_APP_TYPE, "capp");
		if (ret == APP_MANAGER_ERROR_NONE){
			ret = app_info_filter_destroy(handle);
			if (ret == APP_MANAGER_ERROR_NONE)
				dts_pass("utc_app_info_filter_destroy_p", "passed");
			else
				dts_fail("utc_app_info_filter_destroy_p", "failed");
		}
		else
		{
			dts_fail("utc_app_info_filter_destroy_p", "failed");
		}
	}
	else
	{
		dts_fail("utc_app_info_filter_destroy_p", "failed");
	}
}

static void utc_app_info_filter_destroy_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_filter_h handle = NULL;
	ret = app_info_filter_destroy(&handle);

	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_filter_destroy_n", "passed");
	else
		dts_fail("utc_app_info_filter_destroy_n", "failed");
}

static void utc_app_info_filter_add_bool_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_filter_h handle = NULL;
	ret = app_info_filter_create(&handle);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_filter_add_bool(handle, PACKAGE_INFO_PROP_APP_NODISPLAY, 1);
		if (ret == APP_MANAGER_ERROR_NONE){
			ret = app_info_filter_destroy(handle);
			if (ret == APP_MANAGER_ERROR_NONE)
				dts_pass("utc_app_info_filter_add_bool_p", "passed");
			else
				dts_fail("utc_app_info_filter_add_bool_p", "failed");
		}
		else
		{
			dts_fail("utc_app_info_filter_add_bool_p", "failed");
		}
	}
	else
	{
		dts_fail("utc_app_info_filter_add_bool_p", "failed");
	}
}


static void utc_app_info_filter_add_bool_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_filter_h handle = NULL;
	ret = app_info_filter_add_bool(handle, PACKAGE_INFO_PROP_APP_NODISPLAY, 1);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_filter_add_bool_n", "passed");
	else
		dts_fail("utc_app_info_filter_add_bool_n", "failed");
}

static void utc_app_info_filter_add_string_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_filter_h handle = NULL;
	ret = app_info_filter_create(&handle);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_filter_add_string(handle, PACKAGE_INFO_PROP_APP_TYPE, "capp");
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_info_filter_add_string_p", "passed");
		else
			dts_fail("utc_app_info_filter_add_string_p", "failed");
	}
	else
	{
		dts_fail("utc_app_info_filter_add_string_p", "failed");
	}
}

static void utc_app_info_filter_add_string_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_filter_h handle = NULL;
	ret = app_info_filter_add_string(handle, PACKAGE_INFO_PROP_APP_TYPE, "capp");
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_filter_add_string_n", "passed");
	else
		dts_fail("utc_app_info_filter_add_string_n", "failed");
}
static void utc_app_info_filter_foreach_appinfo_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_filter_h handle = NULL;
	ret = app_info_filter_create(&handle);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_info_filter_add_string(handle, PACKAGE_INFO_PROP_APP_TYPE, "capp");
		if (ret == APP_MANAGER_ERROR_NONE){
			ret = app_info_filter_foreach_appinfo(handle, filter_cb, NULL);
			if (ret == APP_MANAGER_ERROR_NONE)
				dts_pass("utc_app_info_filter_foreach_appinfo_p", "passed");
			else
				dts_fail("utc_app_info_filter_foreach_appinfo_p", "failed");
		}
		else
		{
			dts_fail("utc_app_info_filter_foreach_appinfo_p", "failed");
		}
	}
	else
	{
		dts_fail("utc_app_info_filter_foreach_appinfo_p", "failed");
	}

}

static void utc_app_info_filter_foreach_appinfo_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_filter_h handle = NULL;
	ret = app_info_filter_foreach_appinfo(handle, filter_cb, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_filter_foreach_appinfo_n", "passed");
	else
		dts_fail("utc_app_info_filter_foreach_appinfo_n", "failed");
}

static void utc_app_info_metadata_filter_create_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *mkey = "http://developer.samsung.com/tizen/metadata/disablescreenread";
	char *mvalue = "shared/res/720x1280/Splash_Tizen_resolution_720x1280_landscape.p";

	app_info_metadata_filter_h handle;

	ret = app_info_metadata_filter_create(&handle);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_metadata_filter_create_p", "passed");
	else
		dts_fail("utc_app_info_metadata_filter_create_p", "failed");

	app_info_metadata_filter_add(handle, mkey, mvalue);

	app_info_metadata_filter_foreach(handle, filter_cb, NULL);

	app_info_metadata_filter_destroy(handle);
}

static void utc_app_info_metadata_filter_create_n(void)
{
	dts_fail("utc_app_info_metadata_filter_create_n", "failed");
}

static void utc_app_info_metadata_filter_destroy_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *mkey = "http://developer.samsung.com/tizen/metadata/disablescreenread";
	char *mvalue = "shared/res/720x1280/Splash_Tizen_resolution_720x1280_landscape.p";

	app_info_metadata_filter_h handle;

	app_info_metadata_filter_create(&handle);

	app_info_metadata_filter_add(handle, mkey, mvalue);

	app_info_metadata_filter_foreach(handle, filter_cb, NULL);

	ret = app_info_metadata_filter_destroy(handle);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_metadata_filter_destroy_p", "passed");
	else
		dts_fail("utc_app_info_metadata_filter_destroy_p", "failed");
}

static void utc_app_info_metadata_filter_destroy_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_metadata_filter_h handle = NULL;
	ret = app_info_metadata_filter_destroy(handle);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_metadata_filter_destroy_n", "passed");
	else
		dts_fail("utc_app_info_metadata_filter_destroy_n", "failed");
}

static void utc_app_info_metadata_filter_add_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *mkey = "http://developer.samsung.com/tizen/metadata/disablescreenread";
	char *mvalue = "shared/res/720x1280/Splash_Tizen_resolution_720x1280_landscape.p";

	app_info_metadata_filter_h handle;

	app_info_metadata_filter_create(&handle);

	ret = app_info_metadata_filter_add(handle, mkey, mvalue);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_metadata_filter_add_p", "passed");
	else
		dts_fail("utc_app_info_metadata_filter_add_p", "failed");

	app_info_metadata_filter_foreach(handle, filter_cb, NULL);

	app_info_metadata_filter_destroy(handle);
}

static void utc_app_info_metadata_filter_add_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_metadata_filter_h handle = NULL;
	ret = app_info_metadata_filter_add(handle, NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_metadata_filter_add_n", "passed");
	else
		dts_fail("utc_app_info_metadata_filter_add_n", "failed");
}

static void utc_app_info_metadata_filter_foreach_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *mkey = "http://developer.samsung.com/tizen/metadata/disablescreenread";
	char *mvalue = "shared/res/720x1280/Splash_Tizen_resolution_720x1280_landscape.p";

	app_info_metadata_filter_h handle;

	app_info_metadata_filter_create(&handle);

	app_info_metadata_filter_add(handle, mkey, mvalue);

	ret = app_info_metadata_filter_foreach(handle, filter_cb, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_metadata_filter_foreach_p", "passed");
	else
		dts_fail("utc_app_info_metadata_filter_foreach_p", "failed");

	app_info_metadata_filter_destroy(handle);
}

static void utc_app_info_metadata_filter_foreach_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	app_info_metadata_filter_h handle = NULL;
	ret = app_info_metadata_filter_foreach(handle, filter_cb, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_info_metadata_filter_foreach_n", "passed");
	else
		dts_fail("utc_app_info_metadata_filter_foreach_n", "failed");
}
