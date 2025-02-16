# Commenting and Naming Conventions Guide

## 1. **Header Files:**

### **Commenting Macros**
   - Macros should be commented with `/* */` style comments. This helps in distinguishing them clearly.
   - Example:
     ```c
     /* Macro to calculate the area of a circle from radius */
     #define AREA_OF_CIRCLE(radius) (3.1415 * (radius) * (radius))
     ```

### **Commenting Typedefs**
   - For typedefs and similar types, use `/* */` comments, but they should be slightly longer to describe the type and its purpose.
   - Example:
     ```c
     /* Typedef for sensor data structure, which stores temperature and humidity readings */
     typedef struct {
         float temperature;
         float humidity;
     } sensor_data_ts;
     ```

### **Function Declarations**
   - Use the following template to comment function declarations:
   
     ```c
     /**
      * @brief [Brief description of what the function does.]
      * 
      * @param [param_name] [description of the parameter and its purpose]
      * @param [param_name] [description of the parameter and its purpose]
      * @return [return_type] [description of the return value]
      */
     return_type function_name(param_type param1, param_type param2);
     ```
   - **Example:**
     ```c
     /**
      * @brief Converts raw ADC value to resistance.
      * 
      * This function takes the raw ADC reading, converts it to a voltage, 
      * and then calculates the resistance based on a formula.
      * 
      * @param raw_adc The raw ADC value to convert to resistance.
      * @return The calculated resistance in ohms.
      */
     float convertToResistance(int raw_adc);
     ```

## 2. **Source Files:**

### **Dividing Code with Sections**
   - **Use block comments** to separate sections of the source file. This makes the file more organized and easier to navigate.

   - **Example Section Dividers:**
     ```c
     /* STATIC GLOBAL VARIABLES */
     // Code for static global variables
     /* *************************************** */

     /* STATIC FUNCTION PROTOTYPES */
     // Code for static function prototypes
     /* *************************************** */

     /* EXPORTED FUNCTIONS */
     // Code for exported functions (public API)
     /* *************************************** */

     /* STATIC FUNCTIONS IMPLEMENTATIONS */
     // Code for static function implementations
     /* *************************************** */
     ```

   - The section titles can be customized based on the nature of the content.

### **Commenting Inside Functions**
   - Inside functions, use `//` for inline comments that explain specific lines or steps in the function logic.
   
   - **Example:**
     ```c
     float convertToResistance(int raw_adc) {
         if (raw_adc == 0) {
             raw_adc += 1;  // Avoid division by zero
         }

         float v_out = raw_adc * ((float)VCC_VOLTAGE / ADC_MAX_VALUE); // Calculate voltage
         float Rs = ((VCC_VOLTAGE - v_out) / v_out) * LOAD_RESISTANCE; // Calculate resistance in ohms
         return Rs;
     }
     ```

## 3. **Naming Conventions:**

### **File and Folder Naming:**
   - Use **`this_syntax`** (underscore-separated lowercase letters) for file names and folder names.
   - Example:
     - `sensor_config.h`
     - `main_module.c`

### **Variable Naming:**
   - Use **`this_syntax`** (underscore-separated lowercase letters) for variable names.
   - Example:
     - `sensor_data`
     - `temperature_reading`

### **Function Naming:**
   - Use **camelCase** for function names, where the first word is lowercase and the following words are capitalized.
   - The function name should represent the module or context it belongs to, followed by the action.
   - Example:
     - `module_functionCall()`
     - `sensor_readTemperature()`

### **Structures and Other Types:**
   - Use a suffix like **_ts**, **_te**, or similar for structs, enums, and other type definitions to indicate their nature.
   - **Example:**
     - `sensor_data_ts` (structure for storing sensor data)
     - `sensor_configuration_te` (enumeration for sensor configurations)

## 4. **General Guidelines:**

- **Be Consistent**: Follow the conventions strictly across the codebase to maintain readability and consistency.
- **Keep Comments Meaningful**: Comments should add value and explain the reasoning behind complex or non-obvious code.
- **Avoid Over-Commenting**: Don't over-comment simple or self-explanatory lines (e.g., `int i = 0; // Initialize i` is unnecessary).
- **Update Comments**: Always update comments when modifying code to ensure they remain relevant and accurate.

By adhering to these guidelines, your code will be easier to understand, maintain, and collaborate on.
