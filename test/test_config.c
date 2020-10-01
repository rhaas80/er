#include <stdio.h>
#include <stdlib.h>
#include "er.h"
#include "er_util.h"

#include "kvtree.h"
#include "kvtree_util.h"

int
main(int argc, char *argv[]) {
    int rc;
    char *conf_file = NULL;
    kvtree* er_config_values = kvtree_new();

    int old_er_debug = er_debug;
    int old_er_set_size = er_set_size;
    int old_er_mpi_buf_size = er_mpi_buf_size;

    MPI_Init(&argc, &argv);

    rc = ER_Init(conf_file);
    if (rc != ER_SUCCESS) {
        printf("ER_Init() failed (error %d)\n", rc);
        return rc;
    }

    /* check ER configuration settings */
    rc = kvtree_util_set_int(er_config_values, ER_KEY_CONFIG_DEBUG,
                             !old_er_debug);
    if (rc != KVTREE_SUCCESS) {
        printf("kvtree_util_set_int failed (error %d)\n", rc);
        return rc;
    }
    rc = kvtree_util_set_int(er_config_values, ER_KEY_CONFIG_SET_SIZE,
                             old_er_set_size + 1);
    if (rc != KVTREE_SUCCESS) {
        printf("kvtree_util_set_int failed (error %d)\n", rc);
        return rc;
    }
    rc = kvtree_util_set_int(er_config_values, ER_KEY_CONFIG_MPI_BUF_SIZE,
                             old_er_mpi_buf_size + 1);
    if (rc != KVTREE_SUCCESS) {
        printf("kvtree_util_set_int failed (error %d)\n", rc);
        return rc;
    }

    printf("Configuring ER (first set of options)...\n");
    if (ER_Config(er_config_values) == NULL) {
        printf("ER_Config() failed\n");
        return EXIT_FAILURE;
    }

    /* check options we just set */

    if (er_debug != !old_er_debug) {
        printf("ER_Config() failed to set %s: %d != %d\n",
               ER_KEY_CONFIG_DEBUG, er_debug, !old_er_debug);
        return EXIT_FAILURE;
    }

    if (er_set_size != old_er_set_size + 1) {
        printf("ER_Config() failed to set %s: %d != %d\n",
               ER_KEY_CONFIG_SET_SIZE, er_set_size, old_er_set_size);
        return EXIT_FAILURE;
    }

    /* configure remainder of options */
    kvtree_delete(&er_config_values);
    er_config_values = kvtree_new();

    printf("Configuring ER (second set of options)...\n");
    if (ER_Config(er_config_values) == NULL) {
        printf("ER_Config() failed\n");
        return EXIT_FAILURE;
    }

    /* check all values once more */

    if (er_debug != !old_er_debug) {
        printf("ER_Config() failed to set %s: %d != %d\n",
               ER_KEY_CONFIG_DEBUG, er_debug, !old_er_debug); 
        return EXIT_FAILURE;
    }

    if (er_set_size != old_er_set_size + 1) {
        printf("ER_Config() failed to set %s: %d != %d\n",
               ER_KEY_CONFIG_SET_SIZE, er_set_size, old_er_set_size); 
        return EXIT_FAILURE;
    }

    if (er_mpi_buf_size != old_er_mpi_buf_size + 1) {
        printf("ER_Config() failed to set %s: %d != %d\n",
               ER_KEY_CONFIG_MPI_BUF_SIZE, er_mpi_buf_size,
               old_er_mpi_buf_size); 
        return EXIT_FAILURE;
    }

    rc = ER_Finalize();
    if (rc != ER_SUCCESS) {
        printf("ER_Finalize() failed (error %d)\n", rc);
        return rc;
    }

    MPI_Finalize();

    return ER_SUCCESS;
}
