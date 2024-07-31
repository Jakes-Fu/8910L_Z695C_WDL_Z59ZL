#ifndef WSP_HEADERS_H
#define WSP_HEADERS_H

#include "wsp_header_api.h"
#include "Octstr.h"

HTTP_HEADER *wsp_headers_unpack(Octstr *headers, int content_type_present);
Octstr* wsp_headers_pack(HTTP_HEADER *headers_list_ptr, int separate_content_type);

#endif
