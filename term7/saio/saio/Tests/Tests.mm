//
//  Tests.m
//  Tests
//
//  Created by Kirill Gorbachyonok on 10/4/20.
//

#import <XCTest/XCTest.h>
#import "dijkstras_algorithm.hpp"

@interface Tests : XCTestCase

@end

@implementation Tests

- (void)testAdd {
    XCTAssertEqual(Add(2, 3), 5);
}

@end
