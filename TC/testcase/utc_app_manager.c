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

static void utc_app_manager_is_running_p(void);
static void utc_app_manager_is_running_n(void);
static void utc_app_manager_foreach_app_context_p(void);
static void utc_app_manager_foreach_app_context_n(void);
static void utc_app_manager_foreach_app_info_p(void);
static void utc_app_manager_foreach_app_info_n(void);
static void utc_app_manager_get_app_id_p(void);
static void utc_app_manager_get_app_id_n(void);
static void utc_app_manager_get_app_context_p(void);
static void utc_app_manager_get_app_context_n(void);
static void utc_app_manager_get_package_p(void);
static void utc_app_manager_get_package_n(void);
static void utc_app_manager_resume_app_p(void);
static void utc_app_manager_resume_app_n(void);
static void utc_app_manager_set_app_context_event_cb_p(void);
static void utc_app_manager_set_app_context_event_cb_n(void);
static void utc_app_manager_set_app_info_event_cb_p(void);
static void utc_app_manager_set_app_info_event_cb_n(void);
static void utc_app_manager_terminate_app_p(void);
static void utc_app_manager_terminate_app_n(void);

struct tet_testlist tet_testlist[] = {
	{ utc_app_manager_is_running_p, 1},
	{ utc_app_manager_is_running_n, 2},
	{ utc_app_manager_foreach_app_context_p, 1},
	{ utc_app_manager_foreach_app_context_n, 2},
	{ utc_app_manager_foreach_app_info_p, 1},
	{ utc_app_manager_foreach_app_info_n, 2},
	{ utc_app_manager_get_app_id_p, 1},
	{ utc_app_manager_get_app_id_n, 2},
	{ utc_app_manager_get_app_context_p, 1},
	{ utc_app_manager_get_app_context_n, 2},
	{ utc_app_manager_get_package_p, 1},
	{ utc_app_manager_get_package_n, 2},
	{ utc_app_manager_resume_app_p, 1},
	{ utc_app_manager_resume_app_n, 2},
	{ utc_app_manager_set_app_context_event_cb_p, 1},
	{ utc_app_manager_set_app_context_event_cb_n, 2},
	{ utc_app_manager_set_app_info_event_cb_p, 1},
	{ utc_app_manager_set_app_info_event_cb_n, 2},
	{ utc_app_manager_terminate_app_p, 1},
	{ utc_app_manager_terminate_app_n, 2},
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

static bool app_context_cb(app_context_h app_context, void * user_data)
{
	return true;
}

static bool app_info_cb(app_info_h app_info, void * user_data)
{
	return true;
}

static void app_mgr_context_event_cb (app_context_h app_context, app_context_event_e event, void * user_data)
{
}

static void app_mgr_info_event_cb (app_info_h app_info, app_info_event_e event, void * user_data)
{
}

static void utc_app_manager_is_running_p(void)
{
    int ret = APP_MANAGER_ERROR_NONE;
    bool is_running = false;

    ret = app_manager_is_running("org.tizen.indicator", &is_running);

    if(ret == APP_MANAGER_ERROR_NONE){
        dts_pass("utc_app_manager_is_running_p", "passed");
    }else{
        dts_fail("utc_app_manager_is_running_p", "failed");
    }
}

static void utc_app_manager_is_running_n(void)
{
    int ret = APP_MANAGER_ERROR_NONE;
    ret = app_manager_is_running(NULL, NULL);

    if(ret != APP_MANAGER_ERROR_NONE){
        dts_pass("utc_app_manager_is_running_p", "passed");
    }else{
        dts_fail("utc_app_manager_is_running_p", "failed");
    }
}

static void  utc_app_manager_set_app_context_event_cb_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_set_app_context_event_cb(app_mgr_context_event_cb, NULL);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_set_app_context_event_cb_p", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_set_app_context_event_cb_p", "failed");
	}
}

static void  utc_app_manager_set_app_context_event_cb_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_set_app_context_event_cb(NULL, NULL);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_set_app_context_event_cb_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_set_app_context_event_cb_n", "failed");
	}
}

static void  utc_app_manager_set_app_info_event_cb_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_set_app_info_event_cb(app_mgr_info_event_cb, NULL);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_set_app_info_event_cb_p", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_set_app_info_event_cb_p", "failed");
	}
}

static void  utc_app_manager_set_app_info_event_cb_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_set_app_info_event_cb(NULL, NULL);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_set_app_info_event_cb_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_set_app_info_event_cb_n", "failed");
	}
}

static void  utc_app_manager_foreach_app_context_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_foreach_app_context(app_context_cb, NULL);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_foreach_app_context_p", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_foreach_app_context_p", "failed");
	}
}

static void  utc_app_manager_foreach_app_context_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_foreach_app_context(NULL, NULL);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_foreach_app_context_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_foreach_app_context_n", "failed");
	}
}

static void  utc_app_manager_foreach_app_info_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_foreach_app_info(app_info_cb, NULL);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_foreach_app_info_p", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_foreach_app_info_p", "failed");
	}
}

static void  utc_app_manager_foreach_app_info_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;

	ret = app_manager_foreach_app_info(NULL, NULL);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_foreach_app_info_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_foreach_app_info_n", "failed");
	}
}

static void  utc_app_manager_get_app_id_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	ret = app_manager_get_app_id(0, appid);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_get_app_id_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_get_app_id_n", "failed");
	}
}

static void  utc_app_manager_get_app_id_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	pid_t pid;
	char * appid;
	app_context_h app_context;
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	ret = app_context_get_pid(app_context, &pid);
	ret = app_manager_get_app_id(pid, &appid);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_get_app_id_p", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_get_app_id_p", "failed");
	}
}

static void  utc_app_manager_resume_app_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	app_context_h app_context = NULL;

	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	if (ret == APP_MANAGER_ERROR_NONE){
		ret = app_manager_resume_app(app_context);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_manager_resume_app_p", "passed");
		else
			dts_fail("utc_app_manager_resume_app_p", "failed");
	}
	else
		dts_fail("utc_app_manager_resume_app_p", "failed");
}

static void  utc_app_manager_resume_app_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_manager_resume_app(NULL);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_resume_app_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_resume_app_n", "failed");
	}
}

static void  utc_app_manager_terminate_app_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	app_context_h app_context = NULL;

	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	if (ret == APP_MANAGER_ERROR_NONE){
		ret = app_manager_terminate_app(app_context);
		if (ret == APP_MANAGER_ERROR_NONE)
			dts_pass("utc_app_manager_terminate_app_p", "passed");
		else
			dts_fail("utc_app_manager_terminate_app_p", "failed");
	}
	else
		dts_fail("utc_app_manager_terminate_app_p", "failed");
}

static void  utc_app_manager_terminate_app_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_manager_terminate_app(NULL);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_terminate_app_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_terminate_app_n", "failed");
	}
}

static void  utc_app_manager_get_app_context_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	ret = app_manager_get_app_context(NULL, NULL);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_get_app_context_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_get_app_context_n", "failed");
	}

}

static void  utc_app_manager_get_app_context_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char appid[256];
	app_context_h app_context = NULL;

	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	if (ret == APP_MANAGER_ERROR_NONE)
		dts_pass("utc_app_manager_get_app_contaxt_p", "passed");
	else
		dts_fail("utc_app_manager_get_app_context_p", "failed");

}

static void  utc_app_manager_get_package_p(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char * package = NULL;
	pid_t pid;
	app_context_h app_context = NULL;
	ret = app_manager_get_app_context("org.tizen.indicator", &app_context);
	ret = app_context_get_pid(app_context, &pid);
	ret = app_manager_get_package(pid, &package);

	if(ret == APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_get_package_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_get_package_n", "failed");
	}

}

static void  utc_app_manager_get_package_n(void)
{
	int ret = APP_MANAGER_ERROR_NONE;
	char * package = NULL;
	ret = app_manager_get_package(-1, &package);

	if(ret != APP_MANAGER_ERROR_NONE){
		dts_pass("utc_app_manager_get_package_n", "passed");
	}
	else
	{
		dts_fail("utc_app_manager_get_package_n", "failed");
	}
}
