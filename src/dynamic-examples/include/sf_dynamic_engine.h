/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.  You may not use, modify or
 * distribute this program under any other version of the GNU General
 * Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) 2005-2012 Sourcefire, Inc.
 *
 * Author: Steven Sturges
 *
 * Dynamic Library Loading for Snort
 *
 */
#ifndef _SF_DYNAMIC_ENGINE_H_
#define _SF_DYNAMIC_ENGINE_H_

#ifndef WIN32
#include <sys/types.h>
#else
#include <stdint.h>
#endif

#include "sf_dynamic_define.h"
#include "sf_dynamic_meta.h"

/* specifies that a function does not return
 * used for quieting Visual Studio warnings
 */
#ifdef WIN32
#if _MSC_VER >= 1400
#define NORETURN __declspec(noreturn)
#else
#define NORETURN
#endif
#else
#define NORETURN
#endif

/* Function prototype used to evaluate a special OTN */
typedef int (*OTNCheckFunction)(void* pPacket, void* pRule);

/* flowFlag is FLOW_*; check flowFlag iff non-zero */
typedef int (*OTNHasFunction)(void* pRule, DynamicOptionType, int flowFlag);

/* Data struct & function prototype used to get list of
 * Fast Pattern Content information. */
typedef struct _FPContentInfo
{
    char *content;
    int length;
    int offset;
    int depth;
    char noCaseFlag;
    char exception_flag;
    char is_relative;
    char fp;
    char fp_only;
    char uri_buffer;
    uint16_t fp_offset;
    uint16_t fp_length;
    struct _FPContentInfo *next;

} FPContentInfo;

typedef int (*GetDynamicContentsFunction)(void *, int, FPContentInfo **);
typedef int (*GetDynamicPreprocOptFpContentsFunc)(void *, FPContentInfo **);
typedef void (*RuleFreeFunc)(void *);

/* ruleInfo is passed to OTNCheckFunction when the fast pattern matches. */
typedef int (*RegisterRule)(
    uint32_t, uint32_t, void *,
    OTNCheckFunction, OTNHasFunction,
    int, GetDynamicContentsFunction, RuleFreeFunc,
    GetDynamicPreprocOptFpContentsFunc
);
typedef void *(*RegisterBit)(void *);
typedef void (*UnregisterBit)(void *);
typedef int (*CheckFlowbit)(void *, void *);
typedef int (*DetectAsn1)(void *, void *, const uint8_t *);
typedef int (*PreprocOptionEval)(void *p, const uint8_t **cursor, void *dataPtr);
typedef int (*PreprocOptionInit)(char *, char *, void **dataPtr);
typedef void (*PreprocOptionCleanup)(void *dataPtr);
typedef int (*SfUnfold)(const uint8_t *, uint32_t , uint8_t *, uint32_t , uint32_t *);
typedef int (*SfBase64Decode)(uint8_t *, uint32_t , uint8_t *, uint32_t , uint32_t *);
#define PREPROC_OPT_EQUAL       0
#define PREPROC_OPT_NOT_EQUAL   1
typedef uint32_t (*PreprocOptionHash)(void *);
typedef int (*PreprocOptionKeyCompare)(void *, void *);
/* Function prototype for rule options that want to add patterns to the
 * fast pattern matcher */
typedef int (*PreprocOptionFastPatternFunc)
    (void *rule_opt_data, int protocol, int direction, FPContentInfo **info);
typedef int (*PreprocOptionOtnHandler)(void *);
typedef int (*PreprocOptionByteOrderFunc)(void *, int32_t);

typedef int (*RegisterPreprocRuleOpt)(
    char *, PreprocOptionInit, PreprocOptionEval,
    PreprocOptionCleanup, PreprocOptionHash, PreprocOptionKeyCompare,
    PreprocOptionOtnHandler, PreprocOptionFastPatternFunc);
typedef int (*PreprocRuleOptInit)(void *);

typedef void (*SessionDataFree)(void *);
typedef int (*SetRuleData)(void *, void *, uint32_t, SessionDataFree);
typedef void *(*GetRuleData)(void *, uint32_t);
typedef void * (*AllocRuleData)(size_t);
typedef void (*FreeRuleData)(void *);

/* Info Data passed to dynamic engine plugin must include:
 * version
 * Pointer to AltDecodeBuffer
 * Pointer to HTTP URI Buffers
 * Pointer to function to register C Rule
 * Pointer to function to register C Rule flowbits
 * Pointer to function to check flowbit
 * Pointer to function to do ASN1 Detection
 * Pointer to functions to log Messages, Errors, Fatal Errors
 * Directory path
 */
#include "sf_dynamic_common.h"

#define ENGINE_DATA_VERSION 8

typedef void *(*PCRECompileFunc)(const char *, int, const char **, int *, const unsigned char *);
typedef void *(*PCREStudyFunc)(const void *, int, const char **);
typedef int (*PCREExecFunc)(const void *, const void *, const char *, int, int, int, int *, int);
typedef void (*PCRECapture)(const void *, const void *);
typedef void(*PCREOvectorInfo)(int **, int *);

typedef struct _DynamicEngineData
{
    int version;

    SFDataBuffer *altBuffer;
    SFDataPointer *altDetect;
    SFDataPointer *fileDataBuf;
    UriInfo *uriBuffers[HTTP_BUFFER_MAX];

    RegisterRule ruleRegister;
    RegisterBit flowbitRegister;
    CheckFlowbit flowbitCheck;
    DetectAsn1 asn1Detect;
    LogMsgFunc logMsg;
    LogMsgFunc errMsg;
    LogMsgFunc fatalMsg;
    char *dataDumpDirectory;

    PreprocRuleOptInit preprocRuleOptInit;

    SetRuleData setRuleData;
    GetRuleData getRuleData;

    DebugMsgFunc debugMsg;
#ifdef SF_WCHAR
    DebugWideMsgFunc debugWideMsg;
#endif

    char **debugMsgFile;
    int *debugMsgLine;

    PCRECompileFunc pcreCompile;
    PCREStudyFunc pcreStudy;
    PCREExecFunc pcreExec;
    SfUnfold sfUnfold;
    SfBase64Decode sfbase64decode;
    GetAltDetectFunc GetAltDetect;
    SetAltDetectFunc SetAltDetect;
    IsDetectFlagFunc Is_DetectFlag;
    DetectFlagDisableFunc DetectFlag_Disable;

    AllocRuleData allocRuleData;
    FreeRuleData freeRuleData;

    UnregisterBit flowbitUnregister;

    PCRECapture pcreCapture;
    PCREOvectorInfo pcreOvectorInfo;
} DynamicEngineData;

extern DynamicEngineData _ded;

/* Function prototypes for Dynamic Engine Plugins */
void CloseDynamicEngineLibs(void);
void LoadAllDynamicEngineLibs(char *path);
int LoadDynamicEngineLib(char *library_name, int indent);
typedef int (*InitEngineLibFunc)(DynamicEngineData *);
typedef int (*CompatibilityFunc)(DynamicPluginMeta *meta, DynamicPluginMeta *lib);

int InitDynamicEngines(char *);
void RemoveDuplicateEngines(void);
int DumpDetectionLibRules(void);
int ValidateDynamicEngines(void);

/* This was necessary because of static code analysis not recognizing that
 * fatalMsg did not return - use instead of fatalMsg
 */
NORETURN void DynamicEngineFatalMessage(const char *format, ...);

typedef struct _PreprocessorOptionInfo
{
    PreprocOptionInit optionInit;
    PreprocOptionEval optionEval;
    PreprocOptionCleanup optionCleanup;
    void             *data;
    PreprocOptionHash optionHash;
    PreprocOptionKeyCompare optionKeyCompare;
    PreprocOptionOtnHandler otnHandler;
    PreprocOptionFastPatternFunc optionFpFunc;

} PreprocessorOptionInfo;

#endif /* _SF_DYNAMIC_ENGINE_H_ */
