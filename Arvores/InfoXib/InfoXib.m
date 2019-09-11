//
//  UIView+InfoXib.m
//  Arvores
//
//  Created by Marcos Amorim on 26/08/19.
//  Copyright © 2019 Marcos Amorim. All rights reserved.
//

#import "InfoXib.h"

@implementation InfoXib : UIView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    
    if (self) {
        
        NSArray *nibArray = [[NSBundle mainBundle]loadNibNamed:@"InfoXib" owner:self options:nil];
        self = [nibArray objectAtIndex:0];
    }
    
    return self;
}


@end
