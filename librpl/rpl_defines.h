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


#ifndef __has_feature
#define __has_feature(x) 0
#endif


#if __has_feature(nullability)
#define RPL_NULLABLE _Nullable
#define RPL_NONNULL  _Nonnull
#else
#define RPL_NULLABLE
#define RPL_NONNULL
#endif

#if __has_feature(assume_nonnull)
#define RPL_NONNULL_BEGIN _Pragma("clang assume_nonnull begin")
#define RPL_NONNULL_END   _Pragma("clang assume_nonnull end")
#else
#define RPL_NONNULL_BEGIN
#define RPL_NONNULL_END
#endif

#define RPL_HEADER_BEGIN  RPL_NONNULL_BEGIN
#define RPL_HEADER_END    RPL_NONNULL_END

#define RPL_SOURCE_BEGIN  RPL_NONNULL_BEGIN
#define RPL_SOURCE_END    RPL_NONNULL_END


#if __has_feature(attribute_cf_returns_retained)
#define RPL_RETURNS_RETAINED __attribute__((cf_returns_retained))
#else
#define RPL_RETURNS_RETAINED
#endif

#if __has_feature(attribute_cf_returns_not_retained)
#define RPL_RETURNS_NOT_RETAINED \
    __attribute__((cf_returns_not_retained))
#else
#define RPL_RETURNS_NOT_RETAINED
#endif

#if __has_feature(attribute_cf_consumed)
#define RPL_RELEASES_ARGUMENT __attribute__((cf_consumed))
#else
#define RPL_RELEASES_ARGUMENT
#endif


#endif /* __RPL__rpl_defines__h__ */
