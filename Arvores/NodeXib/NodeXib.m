//
//  UIViewController+NodeXib.m
//  Arvores
//
//  Created by Marcos Amorim on 11/11/18.
//  Copyright © 2018 Marcos Amorim. All rights reserved.
//

#import "NodeXib.h"

@implementation NodeXib : UIView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    if (self) {
        
        NSArray *nibArray = [[NSBundle mainBundle]loadNibNamed:@"NodeXib" owner:self options:nil];
        self = [nibArray objectAtIndex:0];
    }
    
    return self;
}


@end
