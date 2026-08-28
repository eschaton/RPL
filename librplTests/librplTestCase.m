//
//  librplTestCase.m
//  librplTests
//
//  Created by Chris Hanson on 8/28/26.
//  Copyright © 2026 Base Hit Ventures LLC. All rights reserved.
//

#import "librplTestCase.h"

#include "librplTestSupport.h"

#include <check.h>


RPL_SOURCE_BEGIN


@implementation librplTestCase {
    BOOL _hasResources;
}

- (void)performCheckTests
{
    NSBundle *bundle = [NSBundle bundleForClass:self.class];
    NSFileManager *dfm = NSFileManager.defaultManager;
    NSString *resourcePath = bundle.resourcePath;
    _hasResources = [dfm fileExistsAtPath:resourcePath];
    if (_hasResources) {
	BOOL changed = [dfm changeCurrentDirectoryPath:resourcePath];
	XCTAssertTrue(changed);
    }

    // Get the test runner

    SRunner *sr = configured_tests_runner();

    // Turn off use of fork for the test suite run.

    srunner_set_fork_status(sr, CK_NOFORK);

    // Run all the tests in the test suite, with verbose output.

    srunner_run_all(sr, CK_VERBOSE);

    // Register any failures with Xcode, to get source annotations.

    int nfailed = srunner_ntests_failed(sr);

    TestResult **results = srunner_failures(sr);
    for (int i = 0; i < nfailed; i++) {
	TestResult *result = results[i];
	XCTIssue *issue = [self issueForCheckTestResult:result];
	[self recordIssue:issue];
    }

    // Clean up

    free(results);
    srunner_free(sr);
}


- (XCTIssue *)issueForCheckTestResult:(TestResult *)result
{
    enum test_result rtype = tr_rtype(result);
    int lno = tr_lno(result);
    const char *lfile = tr_lfile(result);
    const char *msg = tr_msg(result);

    XCTIssueType issueType = ((rtype == CK_FAILURE)
			      ? XCTIssueTypeAssertionFailure
			      : XCTIssueTypeUncaughtException);

    XCTSourceCodeLocation *sourceCodeLocation
	= [[XCTSourceCodeLocation alloc] initWithFilePath:@(lfile)
					       lineNumber:lno];

    XCTSourceCodeContext *sourceCodeContext
	= [[XCTSourceCodeContext alloc] initWithLocation:sourceCodeLocation];

    return [[XCTIssue alloc] initWithType:issueType
		       compactDescription:@(msg)
		      detailedDescription:NULL
			sourceCodeContext:sourceCodeContext
			  associatedError:NULL
			      attachments:@[]
				 severity:XCTIssueSeverityError];
}

- (void)testUsingCheck
{
    [self performCheckTests];
}

@end


RPL_SOURCE_END
