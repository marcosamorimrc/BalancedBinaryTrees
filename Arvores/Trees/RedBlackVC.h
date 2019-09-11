//
//  RedBlack.h
//  Arvores
//
//  Created by Marcos Amorim on 20/11/18.
//  Copyright © 2018 Marcos Amorim. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface RedBlackVC : UIViewController

@property (weak, nonatomic) IBOutlet UILabel *label;
@property (weak, nonatomic) IBOutlet UIScrollView *treeScrollView;
@property (weak, nonatomic) IBOutlet UIView *treeZoomSubView;

@property (weak, nonatomic) IBOutlet UITextField *txtSearch;
@property (weak, nonatomic) IBOutlet UITextField *txtInsert;
@property (weak, nonatomic) IBOutlet UITextField *txtRandomTree;

@property (weak, nonatomic) IBOutlet UIButton *btnSearch;
@property (weak, nonatomic) IBOutlet UIButton *btnInsert;
@property (weak, nonatomic) IBOutlet UIButton *btnRandomTree;

@property (weak, nonatomic) IBOutlet UIButton *btnUndo;
@property (weak, nonatomic) IBOutlet UIButton *btnInfo;
@property (weak, nonatomic) IBOutlet UIButton *btnDeleteTree;

@end
