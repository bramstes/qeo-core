/*
 * Copyright (c) 2014 - Qeo LLC
 *
 * The source code form of this Qeo Open Source Project component is subject
 * to the terms of the Clear BSD license.
 *
 * You can redistribute it and/or modify it under the terms of the Clear BSD
 * License (http://directory.fsf.org/wiki/License:ClearBSD). See LICENSE file
 * for more details.
 *
 * The Qeo Open Source Project also includes third party Open Source Software.
 * See LICENSE file for more details.
 */

/* dds_tsm -- Defines the TypeSupport_meta type and some operations on it. */

#ifndef __dds_tsm_h_
#define	__dds_tsm_h_

#include <stdint.h>
#include "dds/dds_types.h"
#include "dds/dds_dcps.h"

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef XTYPES_USED
#define	LONGDOUBLE
#endif

/** Supported CDR types. */
typedef enum {
	CDR_TYPECODE_UNKNOWN = 0,
	CDR_TYPECODE_SHORT,
	CDR_TYPECODE_USHORT,
	CDR_TYPECODE_LONG,
	CDR_TYPECODE_ULONG,
	CDR_TYPECODE_LONGLONG,
	CDR_TYPECODE_ULONGLONG,
	CDR_TYPECODE_FLOAT,
	CDR_TYPECODE_DOUBLE,
#ifdef LONGDOUBLE
	CDR_TYPECODE_LONGDOUBLE,
#endif
	CDR_TYPECODE_FIXED,
	CDR_TYPECODE_BOOLEAN,
	CDR_TYPECODE_CHAR,
	CDR_TYPECODE_WCHAR,
	CDR_TYPECODE_OCTET,
	CDR_TYPECODE_CSTRING,
	CDR_TYPECODE_WSTRING,
	CDR_TYPECODE_STRUCT,
	CDR_TYPECODE_UNION,
	CDR_TYPECODE_ENUM,
	CDR_TYPECODE_SEQUENCE,
	CDR_TYPECODE_ARRAY,

	CDR_TYPECODE_TYPEREF,

	CDR_TYPECODE_MAX
} CDR_TypeCode_t;

typedef enum {
	TSMFLAG_KEY = 1,	/* Set if field is a key field. */
	TSMFLAG_DYNAMIC = 2,	/* Must be set if the container structure
                                   contains pointers, i.e. unbounded strings
				   sequences, shareable fields and optional
				   fields. */
	TSMFLAG_MUTABLE = 4,	/* Extra container fields can be added later. */
	TSMFLAG_OPTIONAL = 8,	/* Field is optional, i.e. is a pointer field
				   that can be skipped. */
	TSMFLAG_SHARED = 16,	/* Field is a pointer field instead of a real
				   data field. */
	TSMFLAG_GENID = 32      /* Generate the memberID when mutable */
} tsm_flags;

/* An array of this structure is used to register a type with DDS. A generic
   mechanism is used to transform this static description into a fast runtime
   representation. This is done by calling the function ::DDS_DynamicType_register.
   This mechanism minimizes the amount of generated code for type support.
   Generation of the static array that corresponds to a specific type definition
   is relatively easy. */
typedef struct dds_typesupport_meta_st DDS_TypeSupport_meta;
struct dds_typesupport_meta_st {
	CDR_TypeCode_t tc; /**< The type code. */
	tsm_flags flags;   /**< Used to indicate a key field or statically allocated field. */
	const char *name;  /**< The name of the field (informative). */
	size_t size;       /**< The size of the corresponding container or C string (if applicable). */
	size_t offset;     /**< The offset of the field in its corresponding container. */
	unsigned int nelem;/**< The number of elements in the container (if applicable). */
	int label;         /**< The label of a union member. */
	const DDS_TypeSupport_meta *tsm; /* Used when a TYPEREF is instantiated. */
};

/* === Type Support API ===================================================== */

/* API function to initialize the run-time type support based on the meta type
   support array. */
DDS_EXPORT DDS_TypeSupport DDS_DynamicType_register (
	const DDS_TypeSupport_meta *tc
);

/* Free the resources associated with a type. */
DDS_EXPORT void DDS_DynamicType_free (
	DDS_TypeSupport ts
);

/* Free the memory used up by a sample, using the type support metadata. Only
   use this if the allocated memory isn't consecutive. */
DDS_EXPORT void DDS_SampleFree (
	void *sample,
	const DDS_TypeSupport ts,
	int full,
	void (*free)(void *p)
);

/* Make a copy of a data sample. */
DDS_EXPORT void* DDS_SampleCopy (
	const void *sample,
	const DDS_TypeSupport ts
);

/* Check if two samples have an identical content and return 1 if so. */
DDS_EXPORT int DDS_SampleEquals (
	const void *sample,
	const void *other_sample,
	const DDS_TypeSupport ts
);

/* Use this method for setting a callback to generate the member id */
DDS_EXPORT void DDS_set_generate_callback (uint32_t (*f) (const char *));

#ifdef  __cplusplus
}
#endif

#endif /* !__dds_tsm_h_ */

