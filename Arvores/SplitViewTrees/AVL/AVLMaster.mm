//
//  TesteMaster.m
//  Arvores
//
//  Created by Marcos Amorim on 29/08/19.
//  Copyright © 2019 Marcos Amorim. All rights reserved.
//

#import "AVLMaster.h"
#import "AVLDetail.h"
#import <QuartzCore/QuartzCore.h>
#import "AVLDetail.h"

#include "AVLTree.h"
#include "utlIterate.h"
#include "Ordered.h"
#include "TreeNode.h"
#include "BinNode.h"
#include "Hash.h"
#include "Heap.h"

@interface AVLMaster ()<UITextFieldDelegate>{

    BOOL isFirstTimeAppearing;
    
}
@end

@implementation AVLMaster

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    UITapGestureRecognizer *HideKeyboardTapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(hideKeyboard:)];
    [self.view addGestureRecognizer:HideKeyboardTapRecognizer];
    
    _btnSearch.layer.cornerRadius = 6;
    _btnInsert.layer.cornerRadius = 6;
    _btnRandomTree.layer.cornerRadius = 6;
    _btnUndo.layer.cornerRadius = 8;
    _btnInfo.layer.cornerRadius = 8;
    _btnDeleteTree.layer.cornerRadius = 8;
    
    _txtSearch.delegate = self;
    _txtInsert.delegate = self;
    _txtRandomTree.delegate = self;
    
    isFirstTimeAppearing = true;
    
}


- (void)viewDidAppear:(BOOL)animated{
    
    if (isFirstTimeAppearing) {
        self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryOverlay;
        isFirstTimeAppearing = false;
    }
    
}

- (void)hideKeyboard:(UITapGestureRecognizer*)sender {
    
    [_txtSearch resignFirstResponder];
    [_txtInsert resignFirstResponder];
    [_txtRandomTree resignFirstResponder];
}

- (void)EnableUndoButton{
    [_btnUndo setEnabled:true];
}

-(BOOL)textFieldShouldReturn:(UITextField *)textField{
    
    if (textField == _txtSearch) {
        [_btnSearch sendActionsForControlEvents:UIControlEventTouchUpInside];
    }
    if (textField == _txtInsert) {
        [_btnInsert sendActionsForControlEvents:UIControlEventTouchUpInside];
    }
    if (textField == _txtRandomTree) {
        [_btnRandomTree sendActionsForControlEvents:UIControlEventTouchUpInside];
    }

    return true;
}

- (IBAction)btnSearch:(id)sender {
    
    [self hideKeyboard:nil];
    
    int value = [_txtSearch.text integerValue];
    _txtSearch.text = @"";
    
    if (value > 0) {
    
        self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryHidden;
        
        if (self.splitViewController.viewControllers.count > 1) {
            _btnSearch.tag = value;
            AVLDetail *detail = self.splitViewController.viewControllers.lastObject;
            [detail performSelector:@selector(SearchNode:) withObject:sender];
        }
        
        
    }else{
        
        UIAlertController * alert = [UIAlertController
                                     alertControllerWithTitle:@"Erro"
                                     message:[NSString stringWithFormat:@"Por favor escolha um número positivo maior que 0."]
                                     preferredStyle:UIAlertControllerStyleAlert];
        
        UIAlertAction* noButton = [UIAlertAction
                                   actionWithTitle:@"OK"
                                   style:UIAlertActionStyleDefault
                                   handler:^(UIAlertAction * action) {
                                   }];
        
        [alert addAction:noButton];
        
        [self presentViewController:alert animated:YES completion:nil];
        
    }
}

- (IBAction)btnInsert:(id)sender {
    
    
    [self hideKeyboard:nil];
    
    int value = [_txtInsert.text integerValue];
    _txtInsert.text = @"";
    
    if (value > 0) {
    
        self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryHidden;
        
        if (self.splitViewController.viewControllers.count > 1) {
            _btnInsert.tag = value;
            AVLDetail *detail = self.splitViewController.viewControllers.lastObject;
            [detail performSelector:@selector(InsertNode:) withObject:sender];
        }
        
        
    }else{
        
        UIAlertController * alert = [UIAlertController
                                     alertControllerWithTitle:@"Erro"
                                     message:[NSString stringWithFormat:@"Por favor escolha um número positivo maior que 0."]
                                     preferredStyle:UIAlertControllerStyleAlert];
        
        UIAlertAction* noButton = [UIAlertAction
                                   actionWithTitle:@"OK"
                                   style:UIAlertActionStyleDefault
                                   handler:^(UIAlertAction * action) {
                                   }];
        
        [alert addAction:noButton];
        
        [self presentViewController:alert animated:YES completion:nil];
        
    }
}

- (IBAction)btnGenerateTree:(id)sender {
    
    [self hideKeyboard:nil];
    
    int value = [_txtRandomTree.text integerValue];
    _txtRandomTree.text = @"";
    
    if (value > 0) {
        
        self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryHidden;
        
        if (self.splitViewController.viewControllers.count > 1) {
            _btnRandomTree.tag = value;
            AVLDetail *detail = self.splitViewController.viewControllers.lastObject;
            [detail performSelector:@selector(GenerateRandomTree:) withObject:sender];
            _btnUndo.enabled = true;
        }
        
        
    }else{
        
        UIAlertController * alert = [UIAlertController
                                     alertControllerWithTitle:@"Erro"
                                     message:[NSString stringWithFormat:@"Por favor escolha um número positivo maior que 0."]
                                     preferredStyle:UIAlertControllerStyleAlert];
        
        UIAlertAction* noButton = [UIAlertAction
                                   actionWithTitle:@"OK"
                                   style:UIAlertActionStyleDefault
                                   handler:^(UIAlertAction * action) {
                                   }];
        
        [alert addAction:noButton];
        
        [self presentViewController:alert animated:YES completion:nil];
        
    }
}

- (IBAction)btnUndo:(id)sender {
    
    if (self.splitViewController.viewControllers.count > 1) {
        _btnUndo.enabled = false;
        AVLDetail *detail = self.splitViewController.viewControllers.lastObject;
        [detail performSelector:@selector(UndoChanges) withObject:nil];
    }
    
}

- (IBAction)btnInfo:(id)sender {
    
    if (self.splitViewController.viewControllers.count > 1) {
        AVLDetail *detail = self.splitViewController.viewControllers.lastObject;
        [detail performSelector:@selector(showTreeInfo) withObject:nil];
    }

}

- (IBAction)btnDeleteTree:(id)sender {
    
    if (self.splitViewController.viewControllers.count > 1) {
        AVLDetail *detail = self.splitViewController.viewControllers.lastObject;
        [detail performSelector:@selector(deleteTree) withObject:nil];
    }
}

@end
