#ifndef _TOOLS
#define _TOOLS
void download(struct parser * Parsed, struct filesize * fz,
			  struct client_params * CParams, int * recv_length);

void list(struct client_params * CParams);

void upload(struct parser * Parsed, struct client_params * CParams,
			struct filesize * fz, int * recv_length);

void delete(struct parser * Parsed, struct client_params * CParams);
#endif
