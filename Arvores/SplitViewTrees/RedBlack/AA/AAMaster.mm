//
//  AAMaster.m
//  Arvores
//
//  Created by Marcos Amorim on 29/08/19.
//  Copyright © 2019 Marcos Amorim. All rights reserved.
//

#import "AAMaster.h"
#import "AADetail.h"
#import <QuartzCore/QuartzCore.h>
#import "AppDelegate.h"

#include "AATree.h"
#include "utlIterate.h"
#include "Ordered.h"
#include "TreeNode.h"
#include "BinNode.h"
#include "Hash.h"
#include "Heap.h"

@interface AAMaster ()<UITextFieldDelegate, UISplitViewControllerDelegate>{
    
    
    BOOL isFirstTimeAppearing;
    
    CGFloat CONTENT_VIEW_Y;
    CGFloat CONTENT_VIEW_HEIGHT;
    CGFloat CONTENT_VIEW_Y_SMALL;
    CGFloat CONTENT_VIEW_HEIGHT_SMALL;
    
}


@property (strong) AADetail *AADetailViewController;

@end

@implementation AAMaster

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    UISwipeGestureRecognizer *HideMaster = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(hideMaster:)];
    HideMaster.direction = UISwipeGestureRecognizerDirectionLeft;
    [self.view addGestureRecognizer:HideMaster];
    
    UITapGestureRecognizer *HideKeyboardTapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(hideKeyboard)];
    [self.view addGestureRecognizer:HideKeyboardTapRecognizer];
    
    CONTENT_VIEW_Y_SMALL = 75;
    CONTENT_VIEW_HEIGHT_SMALL = 230;
    
    if (self.view.frame.size.height <= 500) {
        CGRect frame = CGRectMake(_contentView.frame.origin.x, 75, _contentView.frame.size.width, 240);
        _contentView.frame = frame;
        
        CONTENT_VIEW_Y_SMALL = 12;
        CONTENT_VIEW_HEIGHT_SMALL = 220;
        
    }
    
    _txtSearch.layer.cornerRadius = 6;
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
    
    self.navigationController.navigationBar.hidden = true;
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillShow:)
                                                 name:UIKeyboardWillShowNotification
                                               object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardDidHide:)
                                                 name:UIKeyboardDidHideNotification
                                               object:nil];
    
}

- (void)awakeFromNib{
    [super awakeFromNib];
    
    self.splitViewController.delegate = self;
    self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModeAllVisible;
    
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    _AADetailViewController = (AADetail *)[storyboard instantiateViewControllerWithIdentifier:@"AADetail"];
}

- (BOOL)splitViewController:(UISplitViewController *)splitViewController collapseSecondaryViewController:(UIViewController *)secondaryViewController ontoPrimaryViewController:(UIViewController *)primaryViewController{
    
    if([splitViewController.navigationController.viewControllers.lastObject isEqual:self]){
        return false;
    }else{
        return true;
    }
}



- (void)viewDidAppear:(BOOL)animated{
    
    if (isFirstTimeAppearing) {
        self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModeAllVisible;
        
        CGRect frame = CGRectMake(_contentView.frame.origin.x, (self.view.frame.size.height - CONTENT_VIEW_HEIGHT_SMALL)/2, _contentView.frame.size.width, CONTENT_VIEW_HEIGHT_SMALL);
        
        _contentView.frame = frame;
        
        CONTENT_VIEW_Y = _contentView.frame.origin.y;
        CONTENT_VIEW_HEIGHT = _contentView.frame.size.height;
        isFirstTimeAppearing = false;
    }
    
    if (self.splitViewController.preferredDisplayMode != UISplitViewControllerDisplayModeAllVisible) {
        [NSObject cancelPreviousPerformRequestsWithTarget:self selector:@selector(showMaster) object:nil];
        [self performSelector:@selector(showMaster) withObject:nil afterDelay:0.01];
        
    }
    
}

-(void)showMaster{
    
    if (self.splitViewController.preferredDisplayMode != UISplitViewControllerDisplayModeAllVisible) {
        self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModeAllVisible;
    }
    
}

- (void)hideMaster:(UISwipeGestureRecognizer*)sender {
    
    if (sender.direction == UISwipeGestureRecognizerDirectionLeft) {
        [UIView animateWithDuration:0.5f animations:^{
            self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryHidden;
        }];
    }
}

- (void)hideKeyboard{
    
    [_txtSearch resignFirstResponder];
    [_txtInsert resignFirstResponder];
    [_txtRandomTree resignFirstResponder];
}

- (void)keyboardWillShow: (NSNotification *) notif{
    
    CGRect frame = CGRectMake(_contentView.frame.origin.x, CONTENT_VIEW_Y_SMALL, _contentView.frame.size.width, CONTENT_VIEW_HEIGHT_SMALL);
    
    if (self.view.frame.size.height <= 500) {
        _lblTree.hidden = true;
        _btnUndo.hidden = true;
        _btnInfo.hidden = true;
        _btnDeleteTree.hidden = true;
    }
    
    [UIView animateWithDuration:0.5f animations:^{
        self->_contentView.frame = frame;
    }];
    
}

- (void)keyboardDidHide: (NSNotification *) notif{
    
    CGRect frame = CGRectMake(_contentView.frame.origin.x, CONTENT_VIEW_Y, _contentView.frame.size.width, CONTENT_VIEW_HEIGHT);
    [UIView animateWithDuration:0.5f animations:^{
        self->_contentView.frame = frame;
    }];
    
    if (self.view.frame.size.height <= 500) {
        _lblTree.hidden = false;
        _btnUndo.hidden = false;
        _btnInfo.hidden = false;
        _btnDeleteTree.hidden = false;
    }
    
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
    
    [self hideKeyboard];
    
    int value = [_txtSearch.text integerValue];
    _txtSearch.text = @"";
    
    if (value > 0) {
        
        if (self.splitViewController.viewControllers.count > 1) {
            _btnSearch.tag = value;
            AADetail *detail = self.splitViewController.viewControllers.lastObject;
            [detail performSelector:@selector(SearchNode:) withObject:sender];
            
        }else{
            if ([self.navigationController.viewControllers.lastObject isEqual:self]) {
                
                _btnSearch.tag = value;
                [self.navigationController pushViewController:_AADetailViewController animated:true];
                [_AADetailViewController performSelector:@selector(SearchNode:) withObject:sender];
                
            }
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
    
    int value = [_txtSearch.text integerValue];
    _txtSearch.text = @"";
    
    if (value > 0) {
        
        _btnUndo.enabled = true;
        
        if (self.splitViewController.viewControllers.count > 1) {
            _btnInsert.tag = value;
            AADetail *detail = self.splitViewController.viewControllers.lastObject;
            [detail performSelector:@selector(InsertNode:) withObject:sender];
        }
        else{
            if ([self.navigationController.viewControllers.lastObject isEqual:self]) {
                
                _btnInsert.tag = value;
                [_AADetailViewController performSelector:@selector(InsertNode:) withObject:sender];
                [self.navigationController pushViewController:_AADetailViewController animated:true];
                
            }
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
    
    [self hideKeyboard];
    
    int value = [_txtSearch.text integerValue];
    _txtSearch.text = @"";
    
    if (value > 0) {
        
        _btnUndo.enabled = true;
        
        if (self.splitViewController.viewControllers.count > 1) {
            self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryHidden;
            _btnRandomTree.tag = value;
            AADetail *detail = self.splitViewController.viewControllers.lastObject;
            [detail performSelector:@selector(GenerateRandomTree:) withObject:sender];
            _btnUndo.enabled = true;
        }else{
            if ([self.navigationController.viewControllers.lastObject isEqual:self]) {
                
                _btnRandomTree.tag = value;
                [_AADetailViewController performSelector:@selector(GenerateRandomTree:) withObject:sender];
                [self.navigationController pushViewController:_AADetailViewController animated:true];
                
            }
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
        AADetail *detail = self.splitViewController.viewControllers.lastObject;
        [detail performSelector:@selector(UndoChanges) withObject:nil];
    }else{
        if ([self.navigationController.viewControllers.lastObject isEqual:self]) {
            [_AADetailViewController performSelector:@selector(UndoChanges) withObject:sender];
            [self.navigationController pushViewController:_AADetailViewController animated:true];
        }
    }
    
}

- (IBAction)btnInfo:(id)sender {
    
    if (self.splitViewController.viewControllers.count > 1) {
        AADetail *detail = self.splitViewController.viewControllers.lastObject;
        [detail performSelector:@selector(showTreeInfo) withObject:nil];
    }else{
        if ([self.navigationController.viewControllers.lastObject isEqual:self]) {
            [self.navigationController pushViewController:_AADetailViewController animated:true];
            [_AADetailViewController performSelector:@selector(showTreeInfo) withObject:sender];
        }
    }
    
}

- (IBAction)btnDeleteTree:(id)sender {
    
    if (self.splitViewController.viewControllers.count > 1) {
        AADetail *detail = self.splitViewController.viewControllers.lastObject;
        [detail performSelector:@selector(deleteTree) withObject:nil];
    }else{
        if ([self.navigationController.viewControllers.lastObject isEqual:self]) {
            [self.navigationController pushViewController:_AADetailViewController animated:true];
            [_AADetailViewController performSelector:@selector(deleteTree) withObject:sender];
        }
    }
}

- (IBAction)btnHelp:(id)sender {
    
    
    if (self.splitViewController.viewControllers.count > 1) {
        
        self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryHidden;
        
        AADetail *detail = self.splitViewController.viewControllers.lastObject;
        [detail performSelector:@selector(showHelpModal) withObject:nil];
    }else{
        
        UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
        UIViewController *modal = [storyboard instantiateViewControllerWithIdentifier:@"HelpModal"];
        
        [self presentViewController:modal animated:YES completion:nil];
        
    }
    
}

@end
