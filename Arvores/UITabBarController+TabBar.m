//
//  UITabBarController+TabBar.m
//  Arvores
//
//  Created by Marcos Amorim on 20/11/18.
//  Copyright © 2018 Marcos Amorim. All rights reserved.
//

#import "UITabBarController+TabBar.h"

@implementation UITabBarController (TabBar)

-(void)viewDidLoad{
    
    [[self.tabBarController.tabBar.items objectAtIndex:0] setTitle:NSLocalizedString(@"BotonMapas", @"comment")];
    
}

@end
