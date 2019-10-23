//
//  HelpModal.m
//  Arvores
//
//  Created by Marcos Amorim on 23/10/19.
//  Copyright © 2019 Marcos Amorim. All rights reserved.
//

#import "HelpModal.h"

@interface HelpModal ()

@end

@implementation HelpModal

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (IBAction)btnExit:(id)sender {
    
    [self dismissViewControllerAnimated:true completion:nil];
    
}



/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
