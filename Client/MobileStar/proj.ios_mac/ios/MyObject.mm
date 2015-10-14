#import "MyObject-C-Interface.h"
#import "MyObject.h"
//
//@implementation MyObject
//
// C "trampoline" function to invoke Objective-C method


#import <Foundation/Foundation.h>
#include "MyObject.h"
@implementation MyObject
//Your objective c code here....

int MyObjectDoSomethingWith (void *self, void *aParameter)
{
    MyObject *obj=[[MyObject alloc]init]; //Allocating the new object for the objective C   class we created
    [obj doSomethingWith:(NULL)];
    return 21;
}


-(int) doSomethingWith:(void *)num1
{
    NSLog(@"Hello!!!!!!");
//    NSNumber *value = [NSNumber numberWithInt:UIInterfaceOrientationPortrait];
//    [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
    
    [[UIDevice currentDevice] setValue:
     [NSNumber numberWithInteger: UIInterfaceOrientationPortrait]
                                forKey:@"orientation"];
    return 21;
}
@end