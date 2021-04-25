//
// Created by SGuan on 4/22/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/catalog/catalog_attr_helper.h"

int attr_has_notnull(struct attr_data* a_data){
    if(a_data->num_of_constr > 0){
        for(int i = 0; i < a_data->num_of_constr; i++){
            if(a_data->constr[i]->type == NOT_NULL){
                return 1;
            }
        }
    }
    return 0;
}

int attr_has_unique(struct attr_data* a_data){
    if(a_data->num_of_constr > 0){
        for(int i = 0; i < a_data->num_of_constr; i++){
            if(a_data->constr[i]->type == UNIQUE){
                return 1;
            }
        }
    }
    return 0;
}