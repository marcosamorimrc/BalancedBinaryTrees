//
//  TesteDetail.h
//  Arvores
//
//  Created by Marcos Amorim on 29/08/19.
//  Copyright © 2019 Marcos Amorim. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface AVLDetail : UIViewController

@property (weak, nonatomic) IBOutlet UIButton *btnBack;

@property (weak, nonatomic) IBOutlet UIScrollView *treeScrollView;
@property (weak, nonatomic) IBOutlet UIView *treeZoomSubView;

-(void)SearchNode:(UIButton*)sender;
-(void)InsertNode:(UIButton*)sender;
-(void)GenerateRandomTree:(UIButton*)sender;

-(IBAction)UndoChanges;
-(void)deleteTree;


@end


NS_ASSUME_NONNULL_END
