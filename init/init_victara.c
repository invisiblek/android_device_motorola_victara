/*
   Copyright (c) 2013, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

// I'm sure this can be simplified a bunch

int parse_option(const char *line, const char *option, char *value, size_t size)                                                                              
{                                                                                                                                                             
    const char *p0, *p1;                                                                                                                                      
    int len;                                                                                                                                                  

    p0 = strstr(line, option);                                                                                                                                
    if (!p0)                                                                                                                                                  
        return 0;                                                                                                                                             
    p0 += strlen(option);                                                                                                                                     
    p1  = strchr(p0, ' ');                                                                                                                                    
    if (!p1)                                                                                                                                                  
       p1 = p0 + strlen(p0);                                                                                                                                  
    len = p1 - p0;                                                                                                                                            
    if (len > size - 1)                                                                                                                                       
        len = size - 1;                                                                                                                                       
    memcpy(value, p0, len);                                                                                                                                   
    value[len] = '\0';                                                                                                                                        
}

void get_cmdline_option(const char *option, char *value, size_t size)                                                                                         
{                                                                                                                                                             
    FILE  *fp;                                                                                                                                                
    char  *line = NULL;                                                                                                                                       
    size_t len = 0;                                                                                                                                           
    size_t read;                                                                                                                                              

    if (!size)                                                                                                                                                
        return;                                                                                                                                               

    *value = '\0';                                                                                                                                            
    fp = fopen("/proc/cmdline", "r");                                                                                                                         

    if (fp == NULL)                                                                                                                                           
         return;                                                                                                                                              

    while ((read = getline(&line, &len, fp)) != -1) {                                                                                                         
        printf("%s", line);                                                                                                                                   
        if (parse_option(line, option, value, size))                                                                                                          
            break;                                                                                                                                            
    }
                                                                                                                                                         
    fclose(fp);                                                                                                                                           

    if (line)                                                                                                                                                 
        free(line);

    return;                                                                                                                                            
}

void vendor_load_properties()
{
    char carrier[3];
    get_cmdline_option("androidboot.carrier=", carrier, sizeof(carrier));

    if (strstr(carrier, "vzw")) {
        /* xt1096 (vzw) */
        property_set("ro.telephony.default_network", "10");
        property_set("telephony.lteOnCdmaDevice", "1");
    } else {
        /* all others */
        property_set("ro.telephony.default_network", "9");
        property_set("telephony.lteOnGsmDevice", "1");
    }
}
