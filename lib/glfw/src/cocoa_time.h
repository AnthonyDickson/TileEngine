

#define GLFW_COCOA_LIBRARY_TIMER_STATE  _GLFWtimerNS   ns;

// Cocoa-specific global timer data
//
typedef struct _GLFWtimerNS
{
    uint64_t        frequency;
} _GLFWtimerNS;

