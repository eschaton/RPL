//
//  rpl_defines.h
//  librpl
//
//  Created by Chris Hanson on 8/27/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#ifndef __RPL__rpl_defines__h__
#define __RPL__rpl_defines__h__


#if defined(__cplusplus) || defined(cplusplus)
#define RPL_EXPORT extern "C"
#else
#define RPL_EXPORT extern
#endif

#ifndef RPL_USE_NULLABILITY
#define RPL_USE_NULLABILITY 1
#endif

#if RPL_USE_NULLABILITY
#define RPL_NULLABLE _Nullable
#define RPL_NONNULL  _Nonnull

#define RPL_NONNULL_BEGIN _Pragma("clang assume_nonnull begin")
#define RPL_NONNULL_END   _Pragma("clang assume_nonnull end")
#else
#define RPL_NULLABLE
#define RPL_NONNULL

#define RPL_NONNULL_BEGIN
#define RPL_NONNULL_END
#endif


#define RPL_HEADER_BEGIN  RPL_NONNULL_BEGIN
#define RPL_HEADER_END    RPL_NONNULL_END

#define RPL_SOURCE_BEGIN  RPL_NONNULL_BEGIN
#define RPL_SOURCE_END    RPL_NONNULL_END


#endif /* __RPL__rpl_defines__h__ */
