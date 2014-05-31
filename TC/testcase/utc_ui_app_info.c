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

static void  utc_ui_app_info_clone_p(void);
static void  utc_ui_app_info_clone_n(void);
static void  utc_ui_app_info_destroy_p(void);
static void  utc_ui_app_info_destroy_n(void);
static void  utc_ui_app_info_get_app_id_p(void);
static void  utc_ui_app_info_get_app_id_n(void);
static void  utc_ui_app_info_get_icon_p(void);
static void  utc_ui_app_info_get_icon_n(void);
static void  utc_ui_app_info_get_label_p(void);
static void  utc_ui_app_info_get_label_n(void);
static void  utc_ui_app_info_get_package_p(void);
static void  utc_ui_app_info_get_package_n(void);
static void  utc_ui_app_info_get_type_p(void);
static void  utc_ui_app_info_get_type_n(void);
static void  utc_ui_app_info_is_equal_p(void);
static void  utc_ui_app_info_is_equal_n(void);


struct tet_testlist tet_testlist[] = {
	{ utc_ui_app_info_clone_p, 1},
	{ utc_ui_app_info_clone_n, 2},
	{ utc_ui_app_info_destroy_p, 1},
	{ utc_ui_app_info_destroy_n, 2},
	{ utc_ui_app_info_get_app_id_p, 1},
	{ utc_ui_app_info_get_app_id_n, 2},
	{ utc_ui_app_info_get_icon_p, 1},
	{ utc_ui_app_info_get_icon_n, 2},
	{ utc_ui_app_info_get_label_p, 1},
	{ utc_ui_app_info_get_label_n, 2},
	{ utc_ui_app_info_get_package_p, 1},
	{ utc_ui_app_info_get_package_n, 2},
	{ utc_ui_app_info_get_type_p, 1},
	{ utc_ui_app_info_get_type_n, 2},
	{ utc_ui_app_info_is_equal_p, 1},
	{ utc_ui_app_info_is_equal_n, 2},
	{NULL,0},
	};

static void startup(void)
{
}

static void cleanup(void)
{
}

static void utc_ui_app_info_clone_p(void)
{
	ui_app_info_h ui_app_info = NULL;
	ui_app_info_h clone = NULL;
	int ret = APP_MANAGER_ERROR_NONE;
	char *appid = "com.samsung.indicator";
	ret = app_manager_get_ui_app_info(appid, &ui_app_info);
	if(ret == APP_MANAGER_ERROR_NONE){
		ret = ui_app_info_clone(&clone, ui_app_info);
		if (ret == APP_MANAGER_ERROR_NONE){
			dts_pass("utc_ui_app_info_clone_p", "passed");
		}
		else
			dts_fail("utc_ui_app_info_clone_p", "failed");
	}
	else
	{
		dts_fail("utc_ui_app_info_clone_p", "failed");
	}

}

static void utc_ui_app_info_clone_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ui_app_info_h *clone = NULL;
	ret = ui_app_info_clone(clone, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_ui_app_info_clone_n", "passed");
	else
		dts_fail("utc_ui_app_info_clone_n", "failed");
}

static void utc_ui_app_info_destroy_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *appid = "com.samsung.calculator";
	ui_app_info_h ui_app_info = NULL;

	ret = app_manager_get_ui_app_info(appid, &ui_app_info);
	if(ret == APP_MANAGER_ERROR_NONE){
		ret = ui_app_info_destroy(ui_app_info);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_ui_app_info_destroy_p", "passed");
		else
			dts_fail("utc_ui_app_info_destroy_p", "failed");
	}
	else
	{
		dts_fail("utc_ui_app_info_destroy_p", "failed");
	}
}

static void utc_ui_app_info_destroy_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = ui_app_info_destroy(NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_ui_app_info_destroy_n", "passed");
	else
		dts_fail("utc_ui_app_info_destroy_n", "failed");
}

static void utc_ui_app_info_get_app_id_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *appid = "com.samsung.calculator";
	ui_app_info_h ui_app_info = NULL;
	char *out_appid = NULL;

	ret = app_manager_get_ui_app_info(appid, &ui_app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = ui_app_info_get_app_id(ui_app_info, &out_appid);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_ui_app_info_get_app_id_p", "passed");

		else
			dts_fail("utc_ui_app_info_get_app_id_p", "failed");
	}
	else
	{
		dts_fail("utc_ui_app_info_get_app_id_p", "failed");
	}
}

static void utc_ui_app_info_get_app_id_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *out_appid = NULL;
	ret = ui_app_info_get_app_id(NULL, &out_appid);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_ui_app_info_get_app_id_n", "passed");
	else
		dts_fail("utc_ui_app_info_get_app_id_n", "failed");
}

static void utc_ui_app_info_get_icon_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *path = NULL;
	char *appid = "com.samsung.calculator";
	ui_app_info_h ui_app_info = NULL;

	ret = app_manager_get_ui_app_info(appid, &ui_app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = ui_app_info_get_icon(ui_app_info, &path);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_ui_app_info_get_icon_p", "passed");

		else
			dts_fail("utc_ui_app_info_get_icon_p", "failed");
	}
	else
	{
		dts_fail("utc_ui_app_info_get_icon_p", "failed");
	}
}

static void utc_ui_app_info_get_icon_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *path = NULL;
	ret = app_info_get_icon(NULL, &path);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_ui_app_info_get_icon_n", "passed");
	else
		dts_fail("utc_ui_app_info_get_icon_n", "failed");
}

static void utc_ui_app_info_get_label_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *label = NULL;
	char *appid = "com.samsung.calculator";
	ui_app_info_h ui_app_info = NULL;

	ret = app_manager_get_ui_app_info(appid, &ui_app_info);
	if(ret == APP_MANAGER_ERROR_NONE){
		ret = ui_app_info_get_label(ui_app_info, &label);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_ui_app_info_get_label_p", "passed");

		else
			dts_fail("utc_ui_app_info_get_label_p", "failed");
	}
	else
	{
		dts_fail("utc_ui_app_info_get_label_p", "failed");
	}
}

static void utc_ui_app_info_get_label_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *label = NULL;
	ret = app_info_get_label(NULL, &label);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_ui_app_info_get_label_n", "passed");
	else
		dts_fail("utc_ui_app_info_get_label_n", "failed");
}

static void utc_ui_app_info_get_package_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *package = NULL;
	char *appid = "com.samsung.calculator";
	ui_app_info_h ui_app_info = NULL;

	ret = app_manager_get_ui_app_info(appid, &ui_app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = ui_app_info_get_package(ui_app_info, &package);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_ui_app_info_get_package_p", "passed");

		else
			dts_fail("utc_ui_app_info_get_package_p", "failed");
	}
	else
	{
		dts_fail("utc_ui_app_info_get_package_p", "failed");
	}
}

static void utc_ui_app_info_get_package_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *package = NULL;
	ret = ui_app_info_get_package(NULL, &package);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_ui_app_info_get_package_n", "passed");
	else
		dts_fail("utc_ui_app_info_get_package_n", "failed");
}

static void utc_ui_app_info_get_type_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *type = NULL;
	char *appid = "com.samsung.calculator";
	ui_app_info_h ui_app_info = NULL;

	ret = app_manager_get_ui_app_info(appid, &ui_app_info);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = ui_app_info_get_package(ui_app_info, &type);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_ui_app_info_get_type_p", "passed");

		else
			dts_fail("utc_ui_app_info_get_type_p", "failed");
	}
	else
	{
		dts_fail("utc_ui_app_info_get_type_p", "failed");
	}
}

static void utc_ui_app_info_get_type_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *type = NULL;
	ret = ui_app_info_get_type(NULL, &type);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_ui_app_info_get_type_n", "passed");
	else
		dts_fail("utc_ui_app_info_get_type_n", "failed");
}

static void utc_ui_app_info_is_equal_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char *appid = "com.samsung.calculator";
	ui_app_info_h lhs = NULL;
	ui_app_info_h rhs = NULL;
	bool equal = false;
	ret = app_manager_get_ui_app_info(appid, &lhs);

	if(ret == APP_MANAGER_ERROR_NONE){
		ret = app_manager_get_ui_app_info(appid, &rhs);
		ret = ui_app_info_is_equal(lhs, rhs, &equal);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_ui_app_info_is_equal_p", "passed");

		else
			dts_fail("utc_ui_app_info_is_equal_p", "failed");
	}
	else
	{
		dts_fail("utc_ui_app_info_is_equal_p", "failed");
	}
}

static void utc_ui_app_info_is_equal_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = ui_app_info_is_equal(NULL, NULL, NULL);
	if (ret != APP_MANAGER_ERROR_NONE)
		dts_pass("utc_ui_app_info_is_equal_n", "passed");
	else
		dts_fail("utc_ui_app_info_is_equal_n", "failed");
}
