//
//  RedBlackDetail.m
//  Arvores
//
//  Created by Marcos Amorim on 29/08/19.
//  Copyright © 2019 Marcos Amorim. All rights reserved.
//

#import "RedBlackDetail.h"
#import "RedBlackMaster.h"
#import "NodeXib.h"
#import "InfoXib.h"
#import "AppDelegate.h"
#import <QuartzCore/QuartzCore.h>

#include "RedBlack.h"
#include "utlIterate.h"
#include "Ordered.h"
#include "TreeNode.h"
#include "BinNode.h"
#include "Hash.h"
#include "Heap.h"

@interface RedBlackDetail () <UIScrollViewDelegate>{
    
    RedBlackTree<TreeNode> tree;
    RedBlackTree<TreeNode> previousTree;
    
    BOOL isFirstTimeAppearing;
    
    NSMutableArray *InfoViews;
    
    CGFloat NODE_WIDTH;
    CGFloat NODE_HEIGHT;
    CGFloat NODE_SPACING;
    
}
@end

#define NODE_WIDTH_REGULAR 72
#define NODE_HEIGHT_REGULAR 70
#define NODE_SPACING_REGULAR 51

#define NODE_WIDTH_SMALL 32
#define NODE_HEIGHT_SMALL 30
#define NODE_SPACING_SMALL 21

#define NODE_WIDTH_SMALLEST 17
#define NODE_HEIGHT_SMALLEST 15
#define NODE_SPACING_SMALLEST 21

@implementation RedBlackDetail

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    isFirstTimeAppearing = true;
    
    InfoViews = [[NSMutableArray alloc] init];
    
    _treeScrollView.delegate = self;
    _treeScrollView.maximumZoomScale = 4;
    _treeScrollView.minimumZoomScale = 0.03;
    
    _btnBack.layer.cornerRadius = 6;
    
    NODE_WIDTH = NODE_WIDTH_REGULAR;
    NODE_HEIGHT = NODE_HEIGHT_REGULAR;
    NODE_SPACING = NODE_SPACING_REGULAR;
    
    UIScreenEdgePanGestureRecognizer * showMasterRecognizer=[[UIScreenEdgePanGestureRecognizer alloc]initWithTarget:self action:@selector(showMaster:)];
    [showMasterRecognizer setEdges:UIRectEdgeLeft];
    [self.view addGestureRecognizer:showMasterRecognizer];
    
    UITapGestureRecognizer * hideKeyboardRecognizer=[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(hideKeyboard)];
    [self.view addGestureRecognizer:hideKeyboardRecognizer];
    
    if (self.splitViewController.viewControllers.count > 1) {
        _btnBack.hidden = true;
    }
    
}

-(void)viewDidAppear:(BOOL)animated{
    
    if (isFirstTimeAppearing) {
        isFirstTimeAppearing = false;
        
        [self updateTree];
    }
    [self checkIfTreeIsGenerated];
}

-(void) checkIfTreeIsGenerated{
    
    BinNode<TreeNode, compare_to<TreeNode>>*root = tree.GetRoot();
    
    int size = tree.Size(root);
    
    if (size <= 0) {
        
        AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
        
        if ([appDelegate.nodeArray count] > 0) {
            
            for (int i = 0; i < appDelegate.nodeArray.count; i++) {
                tree.Insert([appDelegate.nodeArray[i] integerValue]);
            }
            [self updateTree];
            
        }
    }
}

- (void)showMaster:(UIScreenEdgePanGestureRecognizer*)sender {
    
    if (sender.edges == UIRectEdgeLeft && sender.state == 1) {
        self.splitViewController.preferredDisplayMode = UISplitViewControllerDisplayModePrimaryOverlay;
    }
}

-(void)hideKeyboard{
    
    UINavigationController *navVC = (UINavigationController *)(self.splitViewController.viewControllers.firstObject) ;
    RedBlackMaster *master = navVC.viewControllers.firstObject;
    [master performSelector:@selector(hideKeyboard) withObject:nil];
    
}

- (IBAction)btnBack:(id)sender {
    
    [self.navigationController popViewControllerAnimated:true];
    
}

- (UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView
{
    return _treeZoomSubView;
}

- (void)scrollViewDidEndZooming:(UIScrollView *)scrollView withView:(UIView *)view atScale:(float)scale
{
}


- (IBAction)SearchNode:(UIButton*)sender {
    
    CGFloat oldZoomScale = _treeScrollView.zoomScale;
    CGPoint oldOffset = _treeScrollView.contentOffset;
    
    _treeScrollView.zoomScale = 1;
    
    int value = sender.tag;
    for (UIView *view in _treeZoomSubView.subviews) {
        [view removeFromSuperview];
    }
    
    BinNode<TreeNode, compare_to<TreeNode>>*root = tree.GetRoot();
    
    int h = tree.Height(root);
    int size = tree.Size(root);
    
    if (size <= 0) {
        
        UIAlertController * alert = [UIAlertController
                                     alertControllerWithTitle:@"Nó não encontrado"
                                     message:[NSString stringWithFormat:@"O nó %d não está na árvore.", value]
                                     preferredStyle:UIAlertControllerStyleAlert];
        
        
        UIAlertAction* noButton = [UIAlertAction
                                   actionWithTitle:@"OK"
                                   style:UIAlertActionStyleDefault
                                   handler:^(UIAlertAction * action) {
                                   }];
        
        [alert addAction:noButton];
        
        [self presentViewController:alert animated:YES completion:nil];
    }
    
    if (h > 4) {
        if (h> 5) {
            NODE_WIDTH = NODE_WIDTH_SMALLEST;
            NODE_HEIGHT = NODE_HEIGHT_SMALLEST;
            NODE_SPACING = NODE_SPACING_SMALLEST;
        }else{
            NODE_WIDTH = NODE_WIDTH_SMALL;
            NODE_HEIGHT = NODE_HEIGHT_SMALL;
            NODE_SPACING = NODE_SPACING_SMALL;
        }
    }else{
        NODE_WIDTH = NODE_WIDTH_REGULAR;
        NODE_HEIGHT = NODE_HEIGHT_REGULAR;
        NODE_SPACING = NODE_SPACING_REGULAR;
    }
    
    CGFloat width = NODE_WIDTH*(pow(2, h));
    CGFloat height = (NODE_HEIGHT + NODE_SPACING)*(h+1);

    _treeScrollView.contentSize = CGSizeMake(MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    _treeZoomSubView.frame = CGRectMake(0, 0, MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    
    if (_treeScrollView.contentSize.width > _treeScrollView.frame.size.width) {
        _treeScrollView.contentOffset = CGPointMake((_treeScrollView.contentSize.width-_treeScrollView.frame.size.width)/2, 0);
    }
    
    CGRect frame = CGRectMake(0, 16, _treeScrollView.contentSize.width, NODE_HEIGHT + NODE_SPACING);
    
    [self drawNodeInSearch:*root :frame :value];
    
    _treeScrollView.zoomScale = oldZoomScale;
    _treeScrollView.contentOffset = oldOffset;
}


- (IBAction)InsertNode:(UIButton*)sender {
    
    CGFloat oldZoomScale = _treeScrollView.zoomScale;
    CGPoint oldOffset = _treeScrollView.contentOffset;
    
    _treeScrollView.zoomScale = 1;
    
    BinNode<TreeNode, compare_to<TreeNode>>*oldRoot = tree.GetRoot();
    int oldHeight = tree.Height(oldRoot);
    
    int value = sender.tag;
    previousTree = tree;
    tree.Insert(value);
    
    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    if ([appDelegate.nodeArray count] > 0 || oldHeight <= 0) {
        [appDelegate.nodeArray addObject:[NSNumber numberWithInteger:value]];
    }
    
    for (UIView *view in _treeZoomSubView.subviews) {
        [view removeFromSuperview];
    }
    
    BinNode<TreeNode, compare_to<TreeNode>>*root = tree.GetRoot();
    
    int h = tree.Height(root);
    int size = tree.Size(root);
    
    if (h > 4) {
        if (h> 5) {
            NODE_WIDTH = NODE_WIDTH_SMALLEST;
            NODE_HEIGHT = NODE_HEIGHT_SMALLEST;
            NODE_SPACING = NODE_SPACING_SMALLEST;
        }else{
            NODE_WIDTH = NODE_WIDTH_SMALL;
            NODE_HEIGHT = NODE_HEIGHT_SMALL;
            NODE_SPACING = NODE_SPACING_SMALL;
        }
    }else{
        NODE_WIDTH = NODE_WIDTH_REGULAR;
        NODE_HEIGHT = NODE_HEIGHT_REGULAR;
        NODE_SPACING = NODE_SPACING_REGULAR;
    }
    
    CGFloat width = NODE_WIDTH*(pow(2, h));
    CGFloat height = (NODE_HEIGHT + NODE_SPACING)*(h+1);
    
    _treeScrollView.contentSize = CGSizeMake(MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    _treeZoomSubView.frame = CGRectMake(0, 0, MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    
    if (_treeScrollView.contentSize.width > _treeScrollView.frame.size.width) {
        _treeScrollView.contentOffset = CGPointMake((_treeScrollView.contentSize.width-_treeScrollView.frame.size.width)/2, 0);
    }
    
    CGRect frame = CGRectMake(0, 16, _treeScrollView.contentSize.width, NODE_HEIGHT + NODE_SPACING);
    
    [self drawTree:*root];
    
    _treeScrollView.zoomScale = oldZoomScale;
    _treeScrollView.contentOffset = oldOffset;
    
}


- (IBAction)GenerateRandomTree:(UIButton*)sender {
    
    _treeScrollView.zoomScale = 1;
    
    previousTree = tree;
    [self deleteTree];
    
    for (UIView *view in _treeZoomSubView.subviews) {
        [view removeFromSuperview];
    }
    
    int nNodes = sender.tag;
    int range = 8*nNodes;
    
    NSMutableArray *nodeArray = [[NSMutableArray alloc] init];
    int insertion;
    
    while (nodeArray.count < nNodes) {
        insertion = (arc4random()%range)+1;
        if (![nodeArray containsObject:[NSNumber numberWithInteger:insertion]]) {
            [nodeArray addObject:[NSNumber numberWithInteger:insertion]];
            tree.Insert(insertion);
        }
    }
    
    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    appDelegate.nodeArray = nodeArray;
    
    BinNode<TreeNode, compare_to<TreeNode>>*root = tree.GetRoot();
    
    int h = tree.Height(root);
    
    if (h > 4) {
        if (h> 5) {
            NODE_WIDTH = NODE_WIDTH_SMALLEST;
            NODE_HEIGHT = NODE_HEIGHT_SMALLEST;
            NODE_SPACING = NODE_SPACING_SMALLEST;
        }else{
            NODE_WIDTH = NODE_WIDTH_SMALL;
            NODE_HEIGHT = NODE_HEIGHT_SMALL;
            NODE_SPACING = NODE_SPACING_SMALL;
        }
    }else{
        NODE_WIDTH = NODE_WIDTH_REGULAR;
        NODE_HEIGHT = NODE_HEIGHT_REGULAR;
        NODE_SPACING = NODE_SPACING_REGULAR;
    }
    
    CGFloat width = NODE_WIDTH*(pow(2, h));
    CGFloat height = (NODE_HEIGHT + NODE_SPACING)*(h+1);
    
    _treeScrollView.contentSize = CGSizeMake(MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    _treeZoomSubView.frame = CGRectMake(0, 0, MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    
    if (_treeScrollView.contentSize.width > _treeScrollView.frame.size.width) {
        _treeScrollView.contentOffset = CGPointMake((_treeScrollView.contentSize.width-_treeScrollView.frame.size.width)/2, 0);
    }
    
    [self drawTree:*root];
    
}

- (IBAction)UndoChanges{
    
    CGFloat oldZoomScale = _treeScrollView.zoomScale;
    CGPoint oldOffset = _treeScrollView.contentOffset;
    
    _treeScrollView.zoomScale = 1;
    
    tree = previousTree;
    
    for (UIView *view in _treeZoomSubView.subviews) {
        [view removeFromSuperview];
    }
    
    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    [appDelegate.nodeArray removeAllObjects];
    
    BinNode<TreeNode, compare_to<TreeNode>>*root = tree.GetRoot();
    
    int h = tree.Height(root);
    
    if (h > 4) {
        if (h> 5) {
            NODE_WIDTH = NODE_WIDTH_SMALLEST;
            NODE_HEIGHT = NODE_HEIGHT_SMALLEST;
            NODE_SPACING = NODE_SPACING_SMALLEST;
        }else{
            NODE_WIDTH = NODE_WIDTH_SMALL;
            NODE_HEIGHT = NODE_HEIGHT_SMALL;
            NODE_SPACING = NODE_SPACING_SMALL;
        }
    }else{
        NODE_WIDTH = NODE_WIDTH_REGULAR;
        NODE_HEIGHT = NODE_HEIGHT_REGULAR;
        NODE_SPACING = NODE_SPACING_REGULAR;
    }
    
    CGFloat width = NODE_WIDTH*(pow(2, h));
    CGFloat height = (NODE_HEIGHT + NODE_SPACING)*(h+1);
    
    _treeScrollView.contentSize = CGSizeMake(MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    _treeZoomSubView.frame = CGRectMake(0, 0, MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    
    if (_treeScrollView.contentSize.width > _treeScrollView.frame.size.width) {
        _treeScrollView.contentOffset = CGPointMake((_treeScrollView.contentSize.width-_treeScrollView.frame.size.width)/2, 0);
    }
    
    if (h >= 0) {
        [self drawTree:*root];
    }
    
    _treeScrollView.zoomScale = oldZoomScale;
    _treeScrollView.contentOffset = oldOffset;
    
}

-(void)showTreeInfo{
    
    BinNode<TreeNode, compare_to<TreeNode>>*root = tree.GetRoot();
    
    int h = tree.Height(root);
    int size = tree.Size(root);
    
    if (size > 0) {
        
        UIAlertController * alert = [UIAlertController
                                     alertControllerWithTitle:@"Detalhes da árvore"
                                     message:[NSString stringWithFormat:@"Altura da árvore: %d \n Tamanho da árvore: %d", h, size]
                                     preferredStyle:UIAlertControllerStyleAlert];
        
        UIAlertAction* noButton = [UIAlertAction
                                   actionWithTitle:@"OK"
                                   style:UIAlertActionStyleDefault
                                   handler:^(UIAlertAction * action) {
                                   }];
        
        [alert addAction:noButton];
        
        [self presentViewController:alert animated:YES completion:nil];
    }else{
        UIAlertController * alert = [UIAlertController
                                     alertControllerWithTitle:@"Árvore vazia"
                                     message:[NSString stringWithFormat:@"A árvore tem atual está vazia."]
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

-(void)showHelpModal{
    
    UIStoryboard *storyboard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    UIViewController *modal = [storyboard instantiateViewControllerWithIdentifier:@"HelpModal"];
    
    [self presentViewController:modal animated:YES completion:nil];
    
}

-(void)deleteTree{
    
    previousTree = tree;
    tree.MakeEmpty();
    
    UINavigationController *navVC = (UINavigationController *)(self.splitViewController.viewControllers.firstObject) ;
    RedBlackMaster *master = navVC.viewControllers.firstObject;
    [master performSelector:@selector(EnableUndoButton) withObject:nil];
    
    _treeScrollView.zoomScale = 1;
    
    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    [appDelegate.nodeArray removeAllObjects];
    
    for (UIView *view in _treeZoomSubView.subviews) {
        [view removeFromSuperview];
    }
    
}

-(void)deleteNode:(int) value{
    
    CGFloat oldZoomScale = _treeScrollView.zoomScale;
    CGPoint oldOffset = _treeScrollView.contentOffset;
    
    _treeScrollView.zoomScale = 1;
    
    tree.Remove(value);
    
    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    [appDelegate.nodeArray removeAllObjects];
    
    for (UIView *view in _treeZoomSubView.subviews) {
        [view removeFromSuperview];
    }
    
    BinNode<TreeNode, compare_to<TreeNode>>*root = tree.GetRoot();
    
    int h = tree.Height(root);
    
    if (h > 4) {
        if (h> 5) {
            NODE_WIDTH = NODE_WIDTH_SMALLEST;
            NODE_HEIGHT = NODE_HEIGHT_SMALLEST;
            NODE_SPACING = NODE_SPACING_SMALLEST;
        }else{
            NODE_WIDTH = NODE_WIDTH_SMALL;
            NODE_HEIGHT = NODE_HEIGHT_SMALL;
            NODE_SPACING = NODE_SPACING_SMALL;
        }
    }else{
        NODE_WIDTH = NODE_WIDTH_REGULAR;
        NODE_HEIGHT = NODE_HEIGHT_REGULAR;
        NODE_SPACING = NODE_SPACING_REGULAR;
    }
    
    CGFloat width = NODE_WIDTH*(pow(2, h));
    CGFloat height = (NODE_HEIGHT + NODE_SPACING)*(h+1);
    
    _treeScrollView.contentSize = CGSizeMake(MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    _treeZoomSubView.frame = CGRectMake(0, 0, MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    
    if (_treeScrollView.contentSize.width > _treeScrollView.frame.size.width) {
        _treeScrollView.contentOffset = CGPointMake((_treeScrollView.contentSize.width-_treeScrollView.frame.size.width)/2, 0);
    }
    
    if (h >= 0) {
        [self drawTree:*root];
    }
    
    _treeScrollView.zoomScale = oldZoomScale;
    _treeScrollView.contentOffset = oldOffset;
    
}

- (void)tapToDeleteNode:(UITapGestureRecognizer *)recognizer
{
    previousTree = tree;
    
    UINavigationController *navVC = (UINavigationController *)(self.splitViewController.viewControllers.firstObject) ;
    RedBlackMaster *master = navVC.viewControllers.firstObject;
    [master performSelector:@selector(EnableUndoButton) withObject:nil];
    [self deleteNode:[recognizer.view tag]];
}

- (void)holdToShowInfo:(UITapGestureRecognizer *)recognizer
{
    
    BinNode<TreeNode, compare_to<TreeNode>> *root = tree.GetRoot();
    BinNode<TreeNode, compare_to<TreeNode>> *node = [self findNode:recognizer.view.tag :root];
    
    int h = tree.Height(node);
    int size = tree.Size(node);
    
    NSLog(@"h e size : %d - %d", h, size);
    CGPoint location = [recognizer locationInView: self.view];
    
    InfoXib *InfoView = [[InfoXib alloc] init];
    InfoView.lblNodeId.text = [NSString stringWithFormat:@"Nó %ld", (long)recognizer.view.tag];
    InfoView.lblHeight.text = [NSString stringWithFormat:@"%ld", (long)h];
    InfoView.lblSize.text = [NSString stringWithFormat:@"%ld", (long)size];
    InfoView.layer.cornerRadius = 16;
    InfoView.clipsToBounds = true;
    
    if (recognizer.state == UIGestureRecognizerStateBegan)
    {
        NSLog(@"touch UIGestureRecognizerStateBegan");
        CGFloat width = self.view.frame.size.width/3;
        CGFloat height = width/2;
        CGFloat x = location.x + NODE_WIDTH;
        CGFloat y = location.y - 30;
        
        if (location.x > self.view.frame.size.width/2) {
            x = location.x - (width + NODE_WIDTH);
        }
        if (y > self.view.frame.size.height - 240) {
            y = self.view.frame.size.height - 240;
        }else{
            if (y < 165) {
                y = 165;
            }
        }
        
        InfoView.frame = CGRectMake(x, y, width, height);
        
        // border
        [InfoView.layer setBorderColor:[UIColor lightGrayColor].CGColor];
        [InfoView.layer setBorderWidth:1.5f];
        
        // drop shadow
        [InfoView.layer setShadowColor:[UIColor blackColor].CGColor];
        [InfoView.layer setShadowOpacity:0.8];
        [InfoView.layer setShadowRadius:33.0];
        [InfoView.layer setShadowOffset:CGSizeMake(2.0, 2.0)];
        
        [InfoViews addObject:InfoView];
        [self.view addSubview:InfoView];
        
    }
    if (recognizer.state == UIGestureRecognizerStateEnded)
    {
        NSLog(@"touch UIGestureRecognizerStateEnded");
        for (UIView *view in InfoViews) {
            [view removeFromSuperview];
        }
        [InfoViews removeAllObjects];
    }
    
}

-(BinNode<TreeNode, compare_to<TreeNode>> *)findNode:(int) value: (BinNode<TreeNode, compare_to<TreeNode>> *) node{
    
    if( node->GetElement() != NULL){
        
        if (node->GetElement().getData() == value) {
            return node;
        }else{
            if (node->GetElement().getData() > value) {
                
                BinNode<TreeNode, compare_to<TreeNode>> *nextNode = node->GetLeft();
                return [self findNode:value :nextNode];
            }else{
                BinNode<TreeNode, compare_to<TreeNode>> *nextNode = node->GetRight();
                return [self findNode:value :nextNode];
            }
        }
    }
    
    return node;
}

-(void) updateTree{
    
    CGFloat oldZoomScale = _treeScrollView.zoomScale;
    CGPoint oldOffset = _treeScrollView.contentOffset;
    
    _treeScrollView.zoomScale = 1;
    
    for (UIView *view in _treeZoomSubView.subviews) {
        [view removeFromSuperview];
    }
    
    BinNode<TreeNode, compare_to<TreeNode>>*root = tree.GetRoot();
    
    int h = tree.Height(root);
    int size = tree.Size(root);
    
    if (h > 4) {
        if (h> 5) {
            NODE_WIDTH = NODE_WIDTH_SMALLEST;
            NODE_HEIGHT = NODE_HEIGHT_SMALLEST;
            NODE_SPACING = NODE_SPACING_SMALLEST;
        }else{
            NODE_WIDTH = NODE_WIDTH_SMALL;
            NODE_HEIGHT = NODE_HEIGHT_SMALL;
            NODE_SPACING = NODE_SPACING_SMALL;
        }
    }else{
        NODE_WIDTH = NODE_WIDTH_REGULAR;
        NODE_HEIGHT = NODE_HEIGHT_REGULAR;
        NODE_SPACING = NODE_SPACING_REGULAR;
    }
    
    CGFloat width = NODE_WIDTH*(pow(2, h));
    CGFloat height = (NODE_HEIGHT + NODE_SPACING)*(h+1);
    
    _treeScrollView.contentSize = CGSizeMake(MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    _treeZoomSubView.frame = CGRectMake(0, 0, MAX(width, _treeScrollView.frame.size.width), MAX(height, _treeScrollView.frame.size.height));
    
    if (_treeScrollView.contentSize.width > _treeScrollView.frame.size.width) {
        _treeScrollView.contentOffset = CGPointMake((_treeScrollView.contentSize.width-_treeScrollView.frame.size.width)/2, 0);
    }
    
    CGRect frame = CGRectMake(0, 16, _treeScrollView.contentSize.width, NODE_HEIGHT + NODE_SPACING);
    
    [self drawTree:*root];
    
    _treeScrollView.zoomScale = oldZoomScale;
    _treeScrollView.contentOffset = oldOffset;
    
}

-(void)drawTree:(BinNode<TreeNode, compare_to<TreeNode>>)root{
    
    CGRect frame = CGRectMake(0, 16, _treeScrollView.contentSize.width, NODE_HEIGHT + NODE_SPACING);
    
    [self drawNode:root :frame];
    
}

-(void)drawNode:(BinNode<TreeNode, compare_to<TreeNode>>)node :(CGRect)frame{
    
    if(  node.GetElement() != NULL){
        
        NodeXib *cell = [[NodeXib alloc] init];
        
        cell.frame = CGRectMake(frame.origin.x, frame.origin.y, frame.size.width, NODE_HEIGHT + NODE_SPACING);
        
        cell.lblData.text = [NSString stringWithFormat:@"%d", node.GetElement().getData()];
        cell.tag = node.GetElement().getData();
        cell.viewData.tag = node.GetElement().getData();
        
        if (node.Color == 1) {
            cell.lblData.textColor = [UIColor whiteColor];
            cell.lblData.backgroundColor = [UIColor blackColor];
        }else{
            cell.lblData.backgroundColor = [UIColor redColor];
        }
        
        cell.viewData.frame = CGRectMake((cell.frame.size.width - NODE_WIDTH - 2)/2, 0, NODE_WIDTH-2, NODE_HEIGHT);
        
        cell.viewData.layer.cornerRadius = cell.viewData.frame.size.height/2;
        cell.viewData.clipsToBounds = true;
        
        UITapGestureRecognizer *singleFingerTap =
        [[UITapGestureRecognizer alloc] initWithTarget:self
                                                action:@selector(tapToDeleteNode:)];
        [cell.viewData addGestureRecognizer:singleFingerTap];
        
        UILongPressGestureRecognizer *longPressTap =
        [[UILongPressGestureRecognizer alloc] initWithTarget:self
                                                      action:@selector(holdToShowInfo:)];
        [cell.viewData addGestureRecognizer:longPressTap];
        
        [_treeZoomSubView addSubview:cell];
        
        if(  node.GetLeft()->GetElement() != NULL && node.GetLeft() != node.GetLeft()->GetLeft()){
            
            [self drawLineInView:cell :frame :YES];
            
            CGRect leftFrame = CGRectMake(frame.origin.x, frame.origin.y+NODE_HEIGHT + NODE_SPACING, frame.size.width/2, NODE_HEIGHT + NODE_SPACING);
            [self drawNode:*node.GetLeft() :leftFrame];
        }
        if(  node.GetRight()->GetElement() != NULL && node.GetRight() != node.GetRight()->GetRight()){
            
            [self drawLineInView:cell :frame :NO];
            
            CGRect rightFrame = CGRectMake(frame.origin.x + (frame.size.width/2), frame.origin.y+NODE_HEIGHT + NODE_SPACING, frame.size.width/2, NODE_HEIGHT + NODE_SPACING);
            [self drawNode:*node.GetRight() :rightFrame];
        }
        
    }
    
}

-(void)drawNodeInSearch:(BinNode<TreeNode, compare_to<TreeNode>>)node :(CGRect)frame :(int)searchedValue{
    
    if(  node.GetElement() != NULL){
        
        NodeXib *cell = [[NodeXib alloc] init];
        
        cell.frame = CGRectMake(frame.origin.x, frame.origin.y, frame.size.width, NODE_HEIGHT + NODE_SPACING);
        
        cell.lblData.text = [NSString stringWithFormat:@"%d", node.GetElement().getData()];
        cell.tag = node.GetElement().getData();
        cell.viewData.tag = node.GetElement().getData();
        
        if (node.Color == 1) {
            cell.lblData.textColor = [UIColor whiteColor];
            cell.lblData.backgroundColor = [UIColor blackColor];
        }else{
            cell.lblData.backgroundColor = [UIColor redColor];
        }
        
        cell.viewData.frame = CGRectMake((cell.frame.size.width - NODE_WIDTH - 2)/2, 0, NODE_WIDTH-2, NODE_HEIGHT);
        cell.viewData.layer.cornerRadius = cell.viewData.frame.size.height/2;
        cell.viewData.clipsToBounds = true;
        
        if (node.GetElement().getData() == searchedValue) {
            cell.viewData.layer.borderWidth = (NODE_HEIGHT/10)-1;
            cell.viewData.layer.borderColor = [UIColor greenColor].CGColor;
        }
        
        
        UITapGestureRecognizer *singleFingerTap =
        [[UITapGestureRecognizer alloc] initWithTarget:self
                                                action:@selector(tapToDeleteNode:)];
        [cell addGestureRecognizer:singleFingerTap];
        
        UILongPressGestureRecognizer *longPressTap =
        [[UILongPressGestureRecognizer alloc] initWithTarget:self
                                                      action:@selector(holdToShowInfo:)];
        [cell.viewData addGestureRecognizer:longPressTap];
        
        [_treeZoomSubView addSubview:cell];
        
        if(  node.GetLeft()->GetElement() != NULL && node.GetLeft() != node.GetLeft()->GetLeft() ){
            
            CGRect leftFrame = CGRectMake(frame.origin.x, frame.origin.y+NODE_HEIGHT + NODE_SPACING, frame.size.width/2, NODE_HEIGHT + NODE_SPACING);
            
            if (searchedValue < node.GetElement().getData()) {
                [self drawLineInViewInSearch:cell :frame :YES];
                [self drawNodeInSearch:*node.GetLeft() :leftFrame :searchedValue];
            }else{
                [self drawLineInView:cell :frame :YES];
                [self drawNode:*node.GetLeft() :leftFrame];
            }
        }
        if(  node.GetRight()->GetElement() != NULL && node.GetRight() != node.GetRight()->GetRight() ){
            
            
            CGRect rightFrame = CGRectMake(frame.origin.x+(frame.size.width/2), frame.origin.y+NODE_HEIGHT + NODE_SPACING, frame.size.width/2, NODE_HEIGHT + NODE_SPACING);
            
            if (searchedValue > node.GetElement().getData()) {
                [self drawLineInViewInSearch:cell :frame :NO];
                [self drawNodeInSearch:*node.GetRight() :rightFrame :searchedValue];
            }else{
                [self drawLineInView:cell :frame :NO];
                [self drawNode:*node.GetRight() :rightFrame];
            }
            
        }
        
        if ((node.GetLeft() == node.GetLeft()->GetLeft() && node.GetElement().getData() > searchedValue) ||
            (node.GetRight() == node.GetRight()->GetRight() && node.GetElement().getData() < searchedValue)
            ) {
            
            NSLog(@"node data : %d", node.GetElement().getData());
            NSLog(@"searchedvalue : %d", searchedValue);
            
            UIAlertController * alert = [UIAlertController
                                         alertControllerWithTitle:@"Nó não encontrado"
                                         message:[NSString stringWithFormat:@"O nó %d não está na árvore.", searchedValue]
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
    
}

-(void)drawLineInView:(UIView *) view:(CGRect)frame:(BOOL) leftChild {
    
    if (leftChild) {
        CAShapeLayer *shapeLayer = [CAShapeLayer layer];
        UIBezierPath *path = [UIBezierPath bezierPath];
        [path moveToPoint:CGPointMake((frame.size.width/2), NODE_HEIGHT)];
        [path addLineToPoint:CGPointMake((frame.size.width/4), NODE_HEIGHT + NODE_SPACING)];
        shapeLayer.path = path.CGPath;
        shapeLayer.strokeColor = [UIColor darkGrayColor].CGColor;
        [view.layer addSublayer:shapeLayer];
    }else{
        CAShapeLayer *shapeLayer = [CAShapeLayer layer];
        UIBezierPath *path = [UIBezierPath bezierPath];
        [path moveToPoint:CGPointMake((frame.size.width/2), NODE_HEIGHT)];
        [path addLineToPoint:CGPointMake((frame.size.width*0.75), NODE_HEIGHT + NODE_SPACING)];
        shapeLayer.path = path.CGPath;
        shapeLayer.strokeColor = [UIColor darkGrayColor].CGColor;
        [view.layer addSublayer:shapeLayer];
    }
    
}

-(void)drawLineInViewInSearch:(UIView *) view:(CGRect)frame:(BOOL) leftChild {
    
    if (leftChild) {
        CAShapeLayer *shapeLayer = [CAShapeLayer layer];
        UIBezierPath *path = [UIBezierPath bezierPath];
        [path moveToPoint:CGPointMake((frame.size.width/2), NODE_HEIGHT)];
        [path addLineToPoint:CGPointMake((frame.size.width/4), NODE_HEIGHT + NODE_SPACING)];
        shapeLayer.path = path.CGPath;
        shapeLayer.lineWidth = 4;
        shapeLayer.strokeColor = [UIColor greenColor].CGColor;
        [view.layer addSublayer:shapeLayer];
    }else{
        CAShapeLayer *shapeLayer = [CAShapeLayer layer];
        UIBezierPath *path = [UIBezierPath bezierPath];
        [path moveToPoint:CGPointMake((frame.size.width/2), NODE_HEIGHT)];
        [path addLineToPoint:CGPointMake((frame.size.width*0.75), NODE_HEIGHT + NODE_SPACING)];
        shapeLayer.path = path.CGPath;
        shapeLayer.lineWidth = 4;
        shapeLayer.strokeColor = [UIColor greenColor].CGColor;
        [view.layer addSublayer:shapeLayer];
    }
    
}

-(void) printTreeInOrder{
    
    BinNode<TreeNode, compare_to<TreeNode>>*root = tree.GetRoot();
    
    NSArray *tmp = [self getArrayOfDataInOrder:*root];
    
}

-(NSArray *)getArrayOfDataInOrder:(BinNode<TreeNode, compare_to<TreeNode>>)root{
    
    if(  root.GetElement() == NULL){
        return [[NSArray alloc] init];
    }else{
        
        NSMutableArray *nodeArray = [[NSMutableArray alloc] init];
        
        if(  root.GetLeft()->GetElement() != NULL ){
            [nodeArray addObjectsFromArray:[self getArrayOfDataInOrder:*root.GetLeft()]];
        }
        
        NSNumber *data = [NSNumber numberWithInteger:root.GetElement().getData()];
        [nodeArray addObject:data];
        
        if(  root.GetRight()->GetElement() != NULL ){
            [nodeArray addObjectsFromArray:[self getArrayOfDataInOrder:*root.GetRight()]];
        }
        
        NSLog(@"teste nodes : %@", nodeArray);
        return [[NSArray alloc] initWithArray:nodeArray];
        
    }
    
}

@end



