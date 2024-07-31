#ifndef __JSC_H__
#define __JSC_H__

//void jsc_init(void);
int jsc_init(void);

void jsc_fini(void);

void initialize_mm(void);

void finalize_mm(void);

void initialize_jk_log(void);

void finalize_jk_log(void);

void initialize_jk_connmgr(void);

void finalize_jk_connmgr(void);

void initialize_jk_http(void);

void finalize_jk_http(void);

void initialize_jk_pcs(void);

void finalize_jk_pcs(void);

void initialize_jk_fs(void);

void finalize_jk_fs(void);

void initialize_jk_font(void);

void finalize_jk_font(void);

void initialize_jk_video(void);

void finalize_jk_video(void);

#ifdef JB_EVALUATION
void initialize_jk_core(void);
void finalize_jk_core(void);
#endif

#endif
