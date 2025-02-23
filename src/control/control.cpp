#include "control.h"

/* STATIC GLOBAL VARIABLES */
static components_status_ts components_status[CONTROL_COMPONENTS_STATUS_SIZE] = {0};
/* *************************************** */

/* STATIC FUNCTION PROTOTYPES */
/**
 * @brief error to the specified output component.
 *
 * This function forwards error recieved from an input component to one of the
 * defined output components. It returns an error code that can be passed
 * to the Error Manager for handling.
 *
 * @param output_component The ID of the output component to which the error
 *                         is forwarded (e.g., display, serial console).
 * @param error_msg        Pointer to structure containing the information about the error.
 *
 * @return An error message of type `control_error_ts` indicating the
 *         status of the routing operation.
 */
static control_error_ts routeErrorToOutput(control_io_t output_component,
                                           const control_error_ts *error_msg);

/**
 * @brief Initializes input return data structure.
 *
 * Sets up the return data structure with the specified input component 
 * and ID, initializing error handling fields to indicate an invalid input by default.
 *
 * @param input_device Pointer to struct with input component and unique identifier of the input component.
 * @return control_input_data_ts The initialized return data structure.
 */
static control_input_data_ts initializeInputReturnData(const control_device_ts *input_device);

/**
 * @brief Initializes a sensor and updates its status.
 *
 * This function marks the specified sensor as used and attempts to initialize it.
 * If initialization is successful, the sensor is also marked as working.
 *
 * @param sensor The sensor ID to initialize.
 */
static void initSensor(uint8_t sensor);

/**
 * @brief Selects uninitialized components by performing a bitwise XOR operation 
 *        between the "used" and "working" component status.
 * 
 * This function calculates which components are uninitialized by taking the 
 * XOR of the status fields from two different indices in the components_status array:
 * - CONTROL_COMPONENTS_STATUS_USED_INDEX: Represents the components that should be in use.
 * - CONTROL_COMPONENTS_STATUS_WORKING_INDEX: Represents the components that are currently working.
 * 
 * The XOR operation identifies bits that differ, effectively marking components 
 * that are either missing or uninitialized.
 * 
 * @return components_status_ts A structure containing the uninitialized components' status.
 */
static components_status_ts selectUninitialized();

/**
 * @brief Initializes or reinitializes system components.
 * 
 * This function handles the initialization of all system components, including
 * outputs (such as serial console and LCD display), inputs (such as RTC), and 
 * sensors (such as DHT11, BMP280, MQ135, etc.). If reinitialization is requested, 
 * it selectively reinitializes only the components that were not successfully 
 * initialized previously.
 * 
 * @param reinit Flag to determine whether this is the first initialization 
 *               (CONTROL_FIRST_INIT) or a reinitialization attempt (CONTROL_REINIT).
 * 
 * @return CONTROL_INITIALIZATION_SUCCESSFUL if all components are successfully 
 *         initialized, otherwise CONTROL_INITIALIZATION_FAILED.
 */
static bool control_initialize(bool reinit);
/* *************************************** */

/* EXPORTED FUNCTIONS */
bool control_init()
{
    return control_initialize(CONTROL_FIRST_INIT);
}

bool control_reinit()
{
    return control_initialize(CONTROL_REINIT);
}

control_error_code_te control_routeDataToOutput(control_io_t output_component, const control_data_ts *data)
{
    // Initialize error code
    control_error_code_te error_code = ERROR_CODE_INVALID_OUTPUT;
    
    switch (output_component)
    {
    case OUTPUT_DISPLAY:
        // Route data to display and update error code
        error_code = display_displayData(data);
        break;

    case OUTPUT_SERIAL_CONSOLE:
        // Route data to serial console and update error code
        error_code = serial_console_displayData(data);
        break;
    }
    // Default error code is set to ERROR_CODE_INVALID_OUTPUT so there is no need to set it in default.
    return error_code;
}

control_input_data_ts control_fetchDataFromInput(const control_device_ts *input_device)
{
    // Initialize input return data with defaults
    control_input_data_ts return_data = initializeInputReturnData(input_device);

    switch (input_device->io_component)
    {
    case INPUT_SENSORS:
        // Fetch sensor reading and update return data
        sensor_return_ts sensor_return = sensors_getReading(input_device->device_id);
        return_data.error_code = sensor_return.error_code;
        return_data.data.input_return.sensor_reading = sensor_return.sensor_reading;
        break;

    case INPUT_RTC:
        // Fetch RTC data and update return data
        rtc_return_ts rtc_return = rtc_getTime(input_device->device_id);
        return_data.error_code = rtc_return.error_code;
        return_data.data.input_return.rtc_reading = rtc_return.rtc_reading;
        break;

    case INPUT_I2C_SCAN:
        // Fetch I2C scan data and update return data
        i2c_scan_return_ts i2c_scan_return = i2c_scan_getReading(input_device->device_id);
        return_data.error_code = i2c_scan_return.error_code;
        return_data.data.input_return.i2c_scan_reading = i2c_scan_return.i2c_scan_reading;
        break;

    default:
        // Default error code is set to ERROR_CODE_INVALID_INPUT so no need to set it again here.
        break;
    }
    return return_data;
}

void control_handleError(const control_error_ts *error)
{
    control_error_ts error_msg_serial = routeErrorToOutput(OUTPUT_SERIAL_CONSOLE, error);
    if (error_msg_serial.error_code != ERROR_CODE_NO_ERROR)
    {
        (void)routeErrorToOutput(OUTPUT_DISPLAY, &error_msg_serial);
    }
}
/* *************************************** */

// TODO: CHANGE THIS FUNC TO RETURN ERROR CODE ONLY

/* STATIC FUNCTIONS IMPLEMENTATIONS */
static control_error_ts routeErrorToOutput(control_io_t output_component, const control_error_ts *error_msg)
{
    // Pack the error to the error struct
    control_device_ts error_device = {INPUT_ERROR, CONTROL_ID_UNUSED};
    control_data_ts data = {*error_msg, error_device};

    //
    control_device_ts output_device = {IO_UNUSED, CONTROL_ID_UNUSED};
    control_error_ts error_return = {ERROR_CODE_NO_ERROR, output_device};

    switch (output_component)
    {
    case OUTPUT_DISPLAY:
        // Route data to display and update error code
        error_return.error_code = display_displayData(&data);
        output_device.io_component = OUTPUT_DISPLAY;
        break;

    case OUTPUT_SERIAL_CONSOLE:
        // Route data to serial console and update error code
        error_return.error_code = serial_console_displayData(&data);
        output_device.io_component = OUTPUT_SERIAL_CONSOLE;
        break;

    default:
        // Set error code for invalid output
        error_return.error_code = ERROR_CODE_INVALID_OUTPUT;
        break;
    }

    error_return.component = output_device;
    return error_return;
}

static control_input_data_ts initializeInputReturnData(const control_device_ts *input_device)
{
    control_input_data_ts return_data;

    // Initialize data part
    return_data.data.input = *input_device;

    // Initialize error part
    return_data.error_code = ERROR_CODE_INVALID_INPUT;

    return return_data;
}

static void initSensor(uint8_t sensor)
{
    components_status[CONTROL_COMPONENTS_STATUS_USED_INDEX].sensors_status |= (1 << sensor);
    control_error_code_te error_code = sensors_init(sensor);
    if(ERROR_CODE_NO_ERROR == error_code)
    {
        components_status[CONTROL_COMPONENTS_STATUS_WORKING_INDEX].sensors_status |= (1 << sensor);
    }
    else
    {
        control_device_ts sensor_device = {INPUT_SENSORS, sensor};
        control_error_ts error = {error_code, sensor_device};
        control_handleError(&error);
    }
}

static components_status_ts selectUninitialized()
{
    // Initialize return structure with all fields set to zero
    components_status_ts return_status_struct = {0};

    // Compute XOR between "used" and "working" statuses to identify uninitialized components
    return_status_struct.outputs_status = components_status[CONTROL_COMPONENTS_STATUS_USED_INDEX].outputs_status ^ 
                                          components_status[CONTROL_COMPONENTS_STATUS_WORKING_INDEX].outputs_status;

    return_status_struct.other_inputs_status = components_status[CONTROL_COMPONENTS_STATUS_USED_INDEX].other_inputs_status ^ 
                                               components_status[CONTROL_COMPONENTS_STATUS_WORKING_INDEX].other_inputs_status;

    return_status_struct.sensors_status = components_status[CONTROL_COMPONENTS_STATUS_USED_INDEX].sensors_status ^ 
                                          components_status[CONTROL_COMPONENTS_STATUS_WORKING_INDEX].sensors_status;

    return return_status_struct;
}

static bool control_initialize(bool reinit)
{
    control_error_code_te error_code = ERROR_CODE_INIT_FAILED;
    control_device_ts device_to_init = {IO_UNUSED, CONTROL_ID_UNUSED};
    control_error_ts error = {error_code, device_to_init};

    // Re-check uninitialized components if reinitializing
    components_status_ts uninitialized_components;
    if (CONTROL_REINIT == reinit)
    {
        uninitialized_components = selectUninitialized();
    }

#ifdef SERIAL_CONSOLE_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.outputs_status & (1 << SERIAL_CONSOLE_COMPONENT)))
    {
        components_status[CONTROL_COMPONENTS_STATUS_USED_INDEX].outputs_status |= (1 << SERIAL_CONSOLE_COMPONENT);

        error_code = serial_console_init();

        if (ERROR_CODE_NO_ERROR == error_code)
        {
            components_status[CONTROL_COMPONENTS_STATUS_WORKING_INDEX].outputs_status |= (1 << SERIAL_CONSOLE_COMPONENT);
        }
        else
        {
            device_to_init = {OUTPUT_SERIAL_CONSOLE, CONTROL_ID_UNUSED};
            error = {error_code, device_to_init};
            control_handleError(&error);
        }
    }
#endif  

#ifdef LCD_DISPLAY_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.outputs_status & (1 << LCD_DISPLAY_COMPONENT)))
    {
        components_status[CONTROL_COMPONENTS_STATUS_USED_INDEX].outputs_status |= (1 << LCD_DISPLAY_COMPONENT);

        error_code = display_init();

        if (ERROR_CODE_NO_ERROR == error_code)
        {
            components_status[CONTROL_COMPONENTS_STATUS_WORKING_INDEX].outputs_status |= (1 << LCD_DISPLAY_COMPONENT);
        }
        else
        {
            device_to_init = {OUTPUT_DISPLAY, CONTROL_ID_UNUSED};
            error = {error_code, device_to_init};
            control_handleError(&error);
        }
    }
#endif  

#ifdef RTC_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.other_inputs_status & (1 << RTC_COMPONENT)))
    {
        components_status[CONTROL_COMPONENTS_STATUS_USED_INDEX].other_inputs_status |= (1 << RTC_COMPONENT);

        error_code = rtc_init();

        if (ERROR_CODE_NO_ERROR == error_code)
        {
            components_status[CONTROL_COMPONENTS_STATUS_WORKING_INDEX].other_inputs_status |= (1 << RTC_COMPONENT);
        }
        else
        {
            device_to_init = {INPUT_RTC, RTC_DEFAULT_RTC};
            error = {error_code, device_to_init};
            control_handleError(&error);
        }
    }
#endif  

#ifdef DHT11_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.sensors_status & (1 << DHT11_COMPONENT)))
    {
        initSensor(DHT11_COMPONENT);
    }
#endif
#ifdef BMP280_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.sensors_status & (1 << BMP280_COMPONENT)))
    {
        initSensor(BMP280_COMPONENT);
    }
#endif
#ifdef BH1750_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.sensors_status & (1 << BH1750_COMPONENT)))
    {
        initSensor(BH1750_COMPONENT);
    }
#endif
#ifdef MQ135_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.sensors_status & (1 << MQ135_COMPONENT)))
    {
        initSensor(MQ135_COMPONENT);
    }
#endif
#ifdef MQ7_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.sensors_status & (1 << MQ7_COMPONENT)))
    {
        initSensor(MQ7_COMPONENT);
    }
#endif
#ifdef GYML8511_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.sensors_status & (1 << GYML8511_COMPONENT)))
    {
        initSensor(GYML8511_COMPONENT);
    }
#endif
#ifdef ARDUINORAIN_COMPONENT
    if (CONTROL_FIRST_INIT == reinit || CONTROL_COMPONENT_INITIALIZED != (uninitialized_components.sensors_status & (1 << ARDUINORAIN_COMPONENT)))
    {
        initSensor(ARDUINORAIN_COMPONENT);
    }
#endif

    // Check initialization status
    uninitialized_components = selectUninitialized();
    if (uninitialized_components.outputs_status == CONTROL_ALL_INITIALIZED &&
        uninitialized_components.other_inputs_status == CONTROL_ALL_INITIALIZED &&
        uninitialized_components.sensors_status == CONTROL_ALL_INITIALIZED)
    {
        return CONTROL_INITIALIZATION_SUCCESSFUL;
    }

    return CONTROL_INITIALIZATION_FAILED;
}
/* *************************************** */