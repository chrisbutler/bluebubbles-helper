#include <dlfcn.h>
#include <dispatch/dispatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The current Messages app loads its Catalyst frameworks during startup.
// Inject only this loader initially, then resolve the helper's private symbols
// after the application's main queue starts. Loading both dylibs at launch can
// fail before constructors run, even when the symbol exists after app startup.
__attribute__((constructor)) static void loadMessagingHelper(void) {
    const char *configuredPath = getenv("BLUEBUBBLES_HELPER_PATH");
    if (!configuredPath || !configuredPath[0]) {
        fprintf(stderr, "BlueBubbles loader: no helper path configured\n");
        return;
    }
    char *helperPath = strdup(configuredPath);
    if (!helperPath) return;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 2 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        void *helper = dlopen(helperPath, RTLD_NOW | RTLD_GLOBAL);
        if (!helper) fprintf(stderr, "BlueBubbles loader: helper failed to load: %s\n", dlerror());
        else fprintf(stderr, "BlueBubbles loader: helper loaded\n");
        free(helperPath);
    });
}
