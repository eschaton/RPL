//
//  librplTestCase.h
//  librplTests
//
//  Created by Chris Hanson on 8/28/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#import <XCTest/XCTest.h>

#import "librplTestSupport.h"

#include "rpl.h"


RPL_HEADER_BEGIN


/*!
 A test case class that bridges libcheck to XCTest.
 */
@interface librplTestCase : XCTestCase
@end


RPL_HEADER_END
