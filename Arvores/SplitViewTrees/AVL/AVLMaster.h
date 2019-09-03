//
//  TesteMaster.h
//  Arvores
//
//  Created by Marcos Amorim on 29/08/19.
//  Copyright © 2019 Marcos Amorim. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface AVLMaster : UIViewController

@property (weak, nonatomic) IBOutlet UITextField *txtSearch;
@property (weak, nonatomic) IBOutlet UITextField *txtInsert;
@property (weak, nonatomic) IBOutlet UITextField *txtRandomTree;

@property (weak, nonatomic) IBOutlet UIButton *btnSearch;
@property (weak, nonatomic) IBOutlet UIButton *btnInsert;
@property (weak, nonatomic) IBOutlet UIButton *btnRandomTree;

@property (weak, nonatomic) IBOutlet UIButton *btnUndo;
@property (weak, nonatomic) IBOutlet UIButton *btnInfo;
@property (weak, nonatomic) IBOutlet UIButton *btnDeleteTree;

- (void)EnableUndoButton;

@end

NS_ASSUME_NONNULL_END
