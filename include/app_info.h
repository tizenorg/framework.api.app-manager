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

 
#ifndef __TIZEN_APPFW_APP_INFO_H
#define __TIZEN_APPFW_APP_INFO_H

#include <tizen.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @addtogroup CAPI_APP_INFO_MODULE
* @{
*/

/** String property for filtering based on app info*/
#define	PACKAGE_INFO_PROP_APP_ID			"PACKAGE_INFO_PROP_APP_ID"
#define	PACKAGE_INFO_PROP_APP_TYPE			"PACKAGE_INFO_PROP_APP_TYPE"

/** Boolean property for filtering based on app info*/
#define	PACKAGE_INFO_PROP_APP_NODISPLAY			"PACKAGE_INFO_PROP_APP_NODISPLAY"
#define	PACKAGE_INFO_PROP_APP_TASKMANAGE		"PACKAGE_INFO_PROP_APP_TASKMANAGE"

/**
 * @brief Application information handle.
 */
typedef struct app_info_s *app_info_h;

/**
 * @brief Application filter handle.
 */
typedef struct app_info_filter_s *app_info_filter_h;

/**
 * @brief Application metadata filter handle.
 */
typedef struct app_info_metadata_filter_s *app_info_metadata_filter_h;

/**
 * @brief Called to get the application information once for each installed application.
 * @param[in] app_info The application information of each installed application
 * @param[in] user_data The user data passed from the foreach function
 * @return @c true to continue with the next iteration of the loop, \n @c false to break out of the loop.
 * @pre app_manager_foreach_app_info() will invoke this callback.
 * @see app_manager_foreach_app_info()
 */
typedef bool (*app_info_filter_cb) (app_info_h app_info, void *user_data);

/**
 * @brief Specifies the type of function passed to app_info_foreach_metadata()
 * @param[in] metadata_name the name of the metadata
 * @param[in] metadata_value the value of the metadata
 * @param[in] user_data user data passed to app_info_foreach_metadata()
 * @return @c true to continue with the next iteration of the loop, \n @c false to break out of the loop.
 * @pre app_info_foreach_metadata() will invoke this callback.
 * @see app_info_foreach_metadata()
 */
typedef bool (*app_info_metadata_cb) (const char *metadata_key, const char *metadata_value, void *user_data);

/**
 * @brief Enumerations of event type for the application information event
 */
typedef enum
{
	APP_INFO_EVENT_INSTALLED, /**< The application is newly installed */
	APP_INFO_EVENT_UNINSTALLED, /**< The application is uninstalled */
	APP_INFO_EVENT_UPDATED, /**< The application is updated */
} app_info_event_e;


/**
 * @brief Create the application information handle.
 * @param [in] app_id The application id
 * @param [out] app_info The application information handle
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_manager_foreach_app_info()
 * @see app_manager_get_app_info()
 */
int app_info_create(const char *app_id, app_info_h *app_info);

/**
 * @brief Destroys the application information handle and releases all its resources.
 * @param [in] app_info The application information handle
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @see app_manager_foreach_app_info()
 * @see app_manager_get_app_info()
 */
int app_info_destroy(app_info_h app_info);


/**
 * @brief Gets the application ID with the given application context.
 * @remarks @a app_id must be released with free() by you.
 * @param [in] app_info The application information
 * @param [out] app_id The application ID of the given application context
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_DB_FAILED Database error occurred
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_get_app_id(app_info_h app_info, char **app_id);

/**
 * @brief Gets the executable path of the application
 * @remarks @an executable path must be released with free() by you.
 * @param [in] app_info The application information
 * @param [out] exec The executable path	 of the application
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_get_exec(app_info_h app_info, char **exec);

/**
 * @brief Gets the name of the application
 * @remarks @a name must be released with free() by you.
 * @param [in] app_info The application information
 * @param [out] name The label of the application
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_DB_FAILED Database error occurred
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_get_label(app_info_h app_info, char **label);

/**
 * @brief Gets localed name of the application
 * @remarks @a localed name must be released with free() by you.
 * @param [in] app_id The application id
 * @param [in] locale The locale information
 * @param [out] label The localed name of the application
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_get_localed_label(const char *app_id, const char *locale, char **label);

/**
 * @brief Gets the absolute path to the icon image
 * @remarks @a path must be released with free() by you.
 * @param [in] app_info The application information
 * @param [out] path The absolute path to the icon
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_DB_FAILED Database error occurred
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_get_icon(app_info_h app_info, char **path);

/**
 * @brief Gets the absolute path to the icon image
 * @remarks @a path must be released with free() by you.
 * @param [in] app_info The application information
 * @param [out] package The package name
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_DB_FAILED Database error occurred
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_get_package(app_info_h app_info, char **package);

/**
 * @brief Gets the absolute path to the icon image
 * @remarks @a path must be released with free() by you.
 * @param [in] app_info The application information
 * @param [out] type The package type
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_DB_FAILED Database error occurred
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_get_type(app_info_h app_info, char **type);

/**
 * @brief This API gets the list of metadata for a particular application
 * @param [in] app_info The application information
 * @param[in] metadata_func		callback function for list
 * @param[in] user_data	user data to be passed to callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_info_foreach_metadata(app_info_h app_info, app_info_metadata_cb callback, void *user_data);

/**
 * @brief Checks whether application information is nodisply
 * @param [in] app_info The application information
 * @param [out] nodisplay true if the application is nodisplay, otherwise false
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_info_is_nodisplay(app_info_h app_info, bool *nodisplay);

/**
 * @brief Checks whether two application information are equal.
 * @param [in] lhs	The first application information to compare
 * @param [in] rhs	The second application information to compare
 * @param [out] equal true if the application information are equal, otherwise false
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_DB_FAILED Database error occurred
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_is_equal(app_info_h lhs, app_info_h rhs, bool *equal);

/**
 * @brief Checks whether application is enabled.
 * @param [in] app_info The application information
 * @param [out] enabled true if the application is enabled, otherwise false
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_info_is_enabled(app_info_h app_info, bool *enabled);

/**
 * @brief Checks whether application is launched on booting time.
 * @param [in] app_info The application information
 * @param [out] onboot true if the application is launched on booting time, otherwise false
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_info_is_onboot(app_info_h app_info, bool *onboot);

/**
 * @brief Checks whether application is preloaded.
 * @param [in] app_info The application information
 * @param [out] preload true if the application is preloaded, otherwise false
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 */
int app_info_is_preload(app_info_h app_info, bool *preload);

/**
 * @brief Clones the application information handle.
 * @param [out] clone If successful, a newly created application information handle will be returned.
 * @param [in] app_info The application information
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_DB_FAILED Database error occurred
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_clone(app_info_h *clone, app_info_h app_info);

/**
 * @brief Creates the application information filter handle from db.  All filter properties will be ANDed.
 * @param[out] handle		pointer to the application info filter handle.
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_filter_create(app_info_filter_h *handle);

/**
 * @brief destroys the application information filter handle freeing up all the resources
 * @param[in] handle		pointer to the application info filter handle.
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_filter_destroy(app_info_filter_h handle);

/**
 * @brief  adds a boolean filter property to the filter handle
 * @param[in] handle		pointer to the application info filter handle.
 * @param[in] property		integer property name.
 * @param[in] value		value corresponding to the property.
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_filter_add_bool(app_info_filter_h handle, const char *property, const bool value);

/**
 * @brief adds a string filter property to the filter handle
 * @param[in] handle		pointer to the application info filter handle.
 * @param[in] property		integer property name.
 * @param[in] value		value corresponding to the property.
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_filter_add_string(app_info_filter_h handle, const char *property, const char *value);

/**
 * @brief counts the number of filtered apps
 * @param[in]	handle		pointer to the application info filter handle.
 * @param[out]	count		pointer to the variable for counting.
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_IO_ERROR I/O error
 */
int app_info_filter_count_appinfo(app_info_filter_h handle, int *count);

/**
 * @brief executes the user supplied callback function for each application that satisfy the filter conditions
 * @param[in] handle		pointer to the application info filter handle.
 * @param[in] callback		callback function.
 * @param[in] user_data		user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_filter_foreach_appinfo(app_info_filter_h handle, app_info_filter_cb callback, void * user_data);

/**
 * @brief creates the application's metadata  information filter handle from db.
 * @param[out] handle		pointer to the application metadata info filter handle.
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_metadata_filter_create(app_info_metadata_filter_h *handle);

/**
 * @brief destroys the application's metadata  information filter handle.
 * @param[in] handle		pointer to the application info filter handle.
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_metadata_filter_destroy(app_info_metadata_filter_h handle);

/**
 * @brief adds filter condition for the query API.  The query will search the entire application metadata  information collected from
 * the manifest file of all the installed packages. You can specify value as NULL to search based on key only.
 * @param[in] handle		pointer to the application metadata info filter handle.
 * @param[in] key			pointer to metadata key
 * @param[in] value			pointer to metadata value
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_metadata_filter_add(app_info_metadata_filter_h handle, const char *key, const char *value);


/**
 * @brief This API executes the filter query. The query will search the entire application metadata  information collected from
 * the manifest file of all the installed packages. For each application returned by the query, the callback will be called. If callback returns
 * negative value, no more callbacks will be called and API will return.
 * @param[in] handle		pointer to the application metadata info filter handle.
 * @param[in] callback		function pointer to callback
 * @param[in] user_data		pointer to user data
 * @return 0 on success, otherwise a negative error value.
 * @retval #APP_MANAGER_ERROR_NONE Successful
 * @retval #APP_MANAGER_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #APP_MANAGER_ERROR_OUT_OF_MEMORY Out of memory
 */
int app_info_metadata_filter_foreach(app_info_metadata_filter_h handle, app_info_filter_cb callback, void *user_data);


#define __START_OF_DEPRECATED_API
int app_info_get_name(app_info_h app_info, char **name);

typedef struct ui_app_info_s *ui_app_info_h;
typedef struct service_app_info_s *service_app_info_h;
int ui_app_info_destroy(ui_app_info_h ui_app_info);
int ui_app_info_get_app_id(ui_app_info_h ui_app_info, char **app_id);
int ui_app_info_get_label(ui_app_info_h ui_app_info, char **label);
int ui_app_info_get_icon(ui_app_info_h ui_app_info, char **iconpath);
int ui_app_info_is_equal(ui_app_info_h lhs, ui_app_info_h rhs, bool *equal);
int ui_app_info_clone(ui_app_info_h *clone, ui_app_info_h ui_app_info);
int ui_app_info_get_package(ui_app_info_h ui_app_info, char **package);
int ui_app_info_get_type(ui_app_info_h ui_app_info, char **type);
//int ui_app_info_get_category(ui_app_info_h ui_app_info, char **category);
int service_app_info_destroy(service_app_info_h service_app_info);
int service_app_info_get_app_id(service_app_info_h service_app_info, char **app_id);
int service_app_info_get_label(service_app_info_h service_app_info, char **label);
int service_app_info_get_icon(service_app_info_h service_app_info, char **iconpath);
int service_app_info_is_equal(service_app_info_h lhs, service_app_info_h rhs, bool *equal);
int service_app_info_clone(service_app_info_h *clone, service_app_info_h service_app_info);
int service_app_info_get_package(service_app_info_h service_app_info, char **package);
int service_app_info_get_type(service_app_info_h service_app_info, char **type);
int service_app_info_is_onboot(service_app_info_h service_app_info, bool *onboot);
int service_app_info_is_autorestart(service_app_info_h service_app_info, bool *autorestart);
#define __END_OF_DEPRECATED_API

/**
* @}
*/

#ifdef __cplusplus
}
#endif

#endif /* __TIZEN_APPFW_APP_INFO_H */
