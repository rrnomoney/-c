#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 商品信息结构体
typedef struct Product {
    char productId[20];
    char productName[50];
    int price;
    int quantity;
    struct Product* next; // 指向链表中下一个产品的指针
} Product;
// 店铺结构体
typedef struct Shop {
    char shopName[50];
    char ownerName[50];
    Product* products; // 该店铺的商品链表头指针
    struct Shop* left; // 二叉树左子树指针
    struct Shop* right; // 二叉树右子树指针
} Shop;

// 用户结构体
typedef struct User {
    char username[50];
    char password[50];
    int userType; // 1: 卖家，2: 买家
    Shop* shop; // 如果是卖家，指向其店铺；如果是买家，为空
    struct User* next;
} User;



// 全局变量，指向用户链表的头指针
User* userList = NULL;

// 比较两个店铺名称的顺序，字典序的比较，便于后续的店铺二叉树插入
//这里用const char*更好，因为是只读的状态，不会对shopname造成更改
int compareShopNames(const char* shopName1, const char* shopName2) {
    return strcmp(shopName1, shopName2);
}

// 递归插入新店铺到店铺树中，二叉树的插入
void insertShop(Shop* currentShop, Shop* newShop) {
    if (compareShopNames(newShop->shopName, currentShop->shopName) < 0) {
        if (currentShop->left == NULL) {
            currentShop->left = newShop;
        }
        else {
            insertShop(currentShop->left, newShop);
        }
    }
    else {
        if (currentShop->right == NULL) {
            currentShop->right = newShop;
        }
        else {
            insertShop(currentShop->right, newShop);
        }
    }
}

// 递归插入新商品到商品链表中，链表的插入
void insertProduct(Product* currentProduct, Product* newProduct) {
    if (currentProduct->next == NULL) {
        currentProduct->next = newProduct;
    }
    else {
        insertProduct(currentProduct->next, newProduct);
    }
}

// 查找特定店铺的函数，二叉树的搜索，根为空直接返回null
Shop* findShop(Shop* root, const char* shopName) {
    if (root == NULL) {
        return NULL;
    }
    int cmp = compareShopNames(shopName, root->shopName);
    if (cmp < 0) {
        return findShop(root->left, shopName);
    }
    else if (cmp > 0) {
        return findShop(root->right, shopName);
    }
    else {
        return root;//当前递归到的结点，不是指根节点
    }
}

// 注册函数，用户名，密码都不能重复，选择用户类型，成功后，建立新的用户结点
//将新用户插入到用户链表
void registerUser() {
    char username[50];
    char password[50];
    int userType;

    // 输入用户名
    printf("Enter username: ");
    scanf("%s", username);

    // 检查用户名是否已存在
    User* currentUser = userList;
    while (currentUser != NULL) {
        if (strcmp(currentUser->username, username) == 0) {
            printf("User %s already registered! Please choose a different username.\n", username);
            return; // 如果用户名重复，直接返回，不进行注册
        }
        currentUser = currentUser->next;
    }

    // 输入密码
    printf("Enter password: ");
    scanf("%s", password);

    // 检查密码是否与已有用户密码重复
    currentUser = userList;//重新从头结点开始
    while (currentUser != NULL) {
        if (strcmp(currentUser->password, password) == 0) {
            printf("Password already in use! Please choose a different password.\n");
            return; // 如果密码重复，直接返回，不进行注册
        }
        currentUser = currentUser->next;
    }

    // 输入用户类型
    printf("Enter user type (1 for seller, 2 for buyer): ");
    scanf("%d", &userType);

    // 创建新用户节点
    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }//分配失败

    // 设置新用户的信息
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->userType = userType;
    newUser->shop = NULL;

    // 将新用户节点插入到用户列表的开头(无头结点)
    newUser->next = userList;
    userList = newUser;

    printf("Registration successful!\n");
}

// 登录函数，用户名和密码的匹配，链表的遍历，看是否匹配
User* loginUser() {
    char username[50];
    char password[50];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    User* currentUser = userList;
    while (currentUser != NULL) {
        if (strcmp(currentUser->username, username) == 0 && strcmp(currentUser->password, password) == 0) {
            printf("Login successful!\n");
            return currentUser;
        }
        currentUser = currentUser->next;
    }

    printf("Login failed. Invalid username or password.\n");
    return NULL;
}

// 添加店铺，建立newshop结点，输入信息，初始化商品，左右子树结点为空
//调用二叉树的插入函数，使用户可以有多个店铺
void addShop(User* seller) {
    Shop* newShop = (Shop*)malloc(sizeof(Shop));
    if (newShop == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter shop name: ");
    scanf("%s", newShop->shopName);
    printf("Enter owner name: ");
    scanf("%s", newShop->ownerName);

    newShop->products = NULL;
    newShop->left = NULL;
    newShop->right = NULL;

    // 插入新店铺到卖家的店铺树中
    if (seller->shop == NULL) {
        seller->shop = newShop;//做树的根
    }
    else {
        insertShop(seller->shop, newShop);//做左右子树
    }

    printf("Shop added successfully!\n");
}

// 删除店铺(及其左右子树店铺)和商品，，检查是否为空，二叉树递归删除左右子树的同时
//通过商品链表的遍历，将对应商品一一删除
void deleteShop(Shop* shop) {
    if (shop == NULL) {
        return;
    }

    // 递归删除左右子树
    deleteShop(shop->left);
    deleteShop(shop->right);

    // 释放该店铺的商品链表内存
    Product* currentProduct = shop->products;
    while (currentProduct != NULL) {
        Product* temp = currentProduct;       //需要新增变量辅助删除
        currentProduct = currentProduct->next;
        free(temp);
    }
    shop->products = NULL; //以确保不再有未释放的商品链表节点。

    // 释放店铺结构体内存
    free(shop);
}

// 删除卖家的店铺信息，，检查卖家有无店铺，二叉树遍历搜索找到对应的店铺，
//删除目标店铺并重建其子树，然后更新现存的状态
void deleteSellerShop(User* seller) {
    if (seller->shop == NULL) {
        printf("You have no shops to delete!\n");
        return;     //检查店铺根节点是否存在
    }

    char shopName[50];
    printf("Enter shop name to delete: ");
    scanf("%s", shopName);

    Shop* parent = NULL;
    Shop* current = seller->shop;//作为根节点
    Shop* target = NULL;

    // 查找要删除的店铺及其父节点
    while (current != NULL) {
        int cmp = compareShopNames(shopName, current->shopName);
        if (cmp == 0) {
            target = current;
            break;
        }
        parent = current;//更新parent为current
        if (cmp < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    if (target == NULL) {
        printf("Shop not found!\n");
        return;
    }

    // 删除目标店铺并重建其子树，现存的状态
    if (parent == NULL) {
        // 要删除的是根店铺
        seller->shop = NULL;
    }
    else if (parent->left == target) {
        parent->left = NULL;
    }
    else {
        parent->right = NULL;
    }

    deleteShop(target);//具体的删除操作在这里
    printf("Shop %s deleted successfully!\n", shopName);
}


// 添加商品到店铺的商品链表，检查有无店铺，然后二叉遍历店铺找到对应的店铺，
// 递归方法将商品传进商品链表
void addProduct(User* seller) {
    if (seller->shop == NULL) {
        printf("You need to add a shop first!\n");
        return;
    }

    char shopName[50];
    printf("Enter shop name to add product to: ");
    scanf("%s", shopName);

    Shop* shop = findShop(seller->shop, shopName);//找到对应店铺
    if (shop == NULL) {
        printf("Shop not found!\n");
        return;
    }

    Product* newProduct = (Product*)malloc(sizeof(Product));
    if (newProduct == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter product ID: ");
    scanf("%s", newProduct->productId);
    printf("Enter product name: ");
    scanf("%s", newProduct->productName);
    printf("Enter price: ");
    scanf("%d", &newProduct->price);
    printf("Enter quantity: ");
    scanf("%d", &newProduct->quantity);

    newProduct->next = NULL;

    // 将新商品添加到店铺的商品链表中
    if (shop->products == NULL) {
        shop->products = newProduct;
    }
    else {
        insertProduct(shop->products, newProduct);
    }

    printf("Product added successfully!\n");
}

// 删除指定商品，检查商铺，商品是否为空，设立prev指针，遍历链表找到该商品
//看是删除头部还是中间还是尾部
void deleteProduct(Shop* shop, char* productId) {
    if (shop == NULL || shop->products == NULL) {
        printf("Shop or product list is empty!\n");
        return;
    }

    Product* current = shop->products;
    Product* prev = NULL;

    while (current != NULL && strcmp(current->productId, productId) != 0) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev == NULL) {
            // 删除头部商品
            shop->products = current->next;
        }
        else {
            // 删除中间或尾部商品
            prev->next = current->next;
        }
        free(current);
        printf("Product %s deleted successfully!\n", productId);
    }
    else {
        printf("Product %s not found!\n", productId);
    }
}

// 通过商品ID查找和显示商品信息，检查店铺及产品是否为空，遍历二叉树找到对应店铺，
//链表遍历，通过id找到相对应商品
void findProductByID(Shop* shop, char* productId) {
    if (shop == NULL || shop->products == NULL) {
        printf("Shop or product list is empty!\n");
        return;
    }

    Product* currentProduct = shop->products;
    while (currentProduct != NULL) {
        if (strcmp(currentProduct->productId, productId) == 0) {
            printf("Product ID: %s\n", currentProduct->productId);
            printf("Product Name: %s\n", currentProduct->productName);
            printf("Price: %d\n", currentProduct->price);
            printf("Quantity: %d\n", currentProduct->quantity);
            return;
        }
        currentProduct = currentProduct->next;
    }

    printf("Product %s not found!\n", productId);
}

// 深度优先遍历店铺树，以中序方式遍历，起到卖家，买家浏览的作用
//二叉树遍历，商品链表的遍历
void dfsShopTree(Shop* shop) {
    if (shop == NULL) {
        return;
    }
    // 中序遍历：先左子树，再根节点，最后右子树
    dfsShopTree(shop->left);
    printf("Shop Name: %s, Owner: %s\n", shop->shopName, shop->ownerName);
    printf("Products:\n");
    // 遍历店铺的商品链表
    Product* currentProduct = shop->products;
    while (currentProduct != NULL) {
        printf("Product ID: %s, Name: %s, Price: %d, Quantity: %d\n",
            currentProduct->productId, currentProduct->productName,
            currentProduct->price, currentProduct->quantity);
        currentProduct = currentProduct->next;
    }
    printf("\n");
    dfsShopTree(shop->right);
}

// 按价格排序商品链表，通过指针的指针来改变链表顺序，用了插入排序的算法
void sortProductsByPrice(Product** headRef) {
    if (*headRef == NULL || (*headRef)->next == NULL) {
        return;
        //headRef 是一个指向 Product 链表头指针的指针（Product**）。
        //*headRef 是一个指向 Product 链表头节点的指针
        //如果链表为空或只有一个节点，则直接返回，无需排序。

    }

    Product* sorted = NULL;
    Product* current = *headRef;

    while (current != NULL) {
        Product* next = current->next;
        //处理当前节点之前保存下一个节点
        if (sorted == NULL || sorted->price > current->price) {
            current->next = sorted;
            sorted = current;//链表的插入(头部)
        }
        else {
            Product* temp = sorted;
            while (temp->next != NULL && temp->next->price <= current->price) {
                temp = temp->next;
            }
            current->next = temp->next;//链表的插入(中间或尾部)
            temp->next = current;
        }

        current = next;//将当前节点更新为之前保存的 next 节点，
        //以便继续处理链表中的下一个节点。
    }

    *headRef = sorted;
}


//文件的录入
void loadUsers() {
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("Failed to open users file!\n");
        return;
    }

    char username[50];
    char password[50];
    int userType;

    while (fscanf(file, "%s %s %d", username, password, &userType) == 3) {
        User* newUser = (User*)malloc(sizeof(User));
        if (newUser == NULL) {
            printf("Memory allocation failed!\n");
            fclose(file);
            return;
        }
        strcpy(newUser->username, username);
        strcpy(newUser->password, password);
        newUser->userType = userType;
        newUser->shop = NULL;
        newUser->next = userList;
        userList = newUser;
    }

    fclose(file);
    printf("Users loaded successfully!\n");
}
void loadShops() {
    FILE* file = fopen("shops.txt", "r");
    if (file == NULL) {
        printf("Failed to open shops file!\n");
        return;
    }

    char shopName[50];
    char ownerName[50];

    while (fscanf(file, "%s %s", shopName, ownerName) == 2) {
        // 查找店主
        User* seller = userList;
        while (seller != NULL && strcmp(seller->username, ownerName) != 0) {
            seller = seller->next;
        }

        if (seller == NULL) {
            printf("Owner %s not found!\n", ownerName);
            continue;
        }

        Shop* newShop = (Shop*)malloc(sizeof(Shop));
        if (newShop == NULL) {
            printf("Memory allocation failed!\n");
            fclose(file);
            return;
        }
        strcpy(newShop->shopName, shopName);
        strcpy(newShop->ownerName, ownerName);
        newShop->products = NULL;
        newShop->left = NULL;
        newShop->right = NULL;

        if (seller->shop == NULL) {
            seller->shop = newShop;
        }
        else {
            insertShop(seller->shop, newShop);
        }
    }

    fclose(file);
    printf("Shops loaded successfully!\n");
}
void loadProducts() {
    FILE* file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("Failed to open products file!\n");
        return;
    }

    char shopName[50];
    char productId[20];
    char productName[50];
    int price;
    int quantity;

    while (fscanf(file, "%s %s %s %d %d", shopName, productId, productName, &price, &quantity) == 5) {
        // 查找店铺
        User* seller = userList;
        Shop* shop = NULL;

        while (seller != NULL && shop == NULL) {
            shop = findShop(seller->shop, shopName);
            seller = seller->next;
        }

        if (shop == NULL) {
            printf("Shop %s not found!\n", shopName);
            continue;
        }

        Product* newProduct = (Product*)malloc(sizeof(Product));
        if (newProduct == NULL) {
            printf("Memory allocation failed!\n");
            fclose(file);
            return;
        }
        strcpy(newProduct->productId, productId);
        strcpy(newProduct->productName, productName);
        newProduct->price = price;
        newProduct->quantity = quantity;
        newProduct->next = NULL;

        if (shop->products == NULL) {
            shop->products = newProduct;
        }
        else {
            insertProduct(shop->products, newProduct);
        }
    }

    fclose(file);
    printf("Products loaded successfully!\n");
}



// 主函数
int main() {
    int choice;
    User* currentUser = NULL;
    loadUsers();
    loadShops();
    loadProducts();
    do {
        printf("\n----- User Menu -----\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Add shop\n");
        printf("4. Add product\n");
        printf("5. Delete shop\n");
        printf("6. Delete product\n");       //后输入店铺遍历
        printf("7. Find product by ID\n");   //后输入店铺，遍历
        printf("8. Display shops (DFS)\n");
        printf("9. Sort products by price\n");
        printf("10. Logout\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            registerUser();
            break;
        case 2:
            currentUser = loginUser();//需要有个当前用户接受返回值
            break;
        case 3:
            if (currentUser != NULL && currentUser->userType == 1) {
                addShop(currentUser);
            }
            else {
                printf("Login as a seller to add a shop!\n");
            }
            break;
        case 4:
            if (currentUser != NULL && currentUser->userType == 1) {
                addProduct(currentUser);
            }
            else {
                printf("Login as a seller to add a product!\n");
            }
            break;
        case 5:
            if (currentUser != NULL && currentUser->userType == 1) {
                deleteSellerShop(currentUser);
            }
            else {
                printf("Login as a seller to delete a shop!\n");
            }
            break;
        case 6:         //二叉树的搜索
            if (currentUser != NULL && currentUser->userType == 1) {
                char shopName[50];
                printf("Enter shop name to delete product from: ");
                scanf("%s", shopName);

                Shop* shop = findShop(currentUser->shop, shopName);
                if (shop == NULL) {
                    printf("Shop not found!\n");
                    break;
                }

                char productId[20];
                printf("Enter product ID to delete: ");
                scanf("%s", productId);
                deleteProduct(shop, productId);
            }
            else {
                printf("Login as a seller to delete a product!\n");
            }
            break;
        case 7:
            if (currentUser != NULL) {
                char shopName[50];
                printf("Enter shop name to find product in: ");
                scanf("%s", shopName);
                //二叉树的遍历
                Shop* shop = findShop(currentUser->shop, shopName);
                if (shop == NULL) {
                    printf("Shop not found!\n");
                    break;
                }

                char productId[20];
                printf("Enter product ID to find: ");
                scanf("%s", productId);
                findProductByID(shop, productId);
            }
            else {
                printf("Login first to find a product!\n");
            }
            break;
        case 8:
            if (currentUser != NULL) {
                printf("\n----- Shops and Products (DFS) -----\n");
                if (currentUser->shop != NULL) {
                    dfsShopTree(currentUser->shop);
                }
                else {
                    printf("No shop added yet!\n");
                }
            }
            else {
                printf("Login first to display shops!\n");
            }
            break;
        case 9:
            if (currentUser != NULL && currentUser->userType == 1) {
                char shopName[50];
                printf("Enter shop name to sort products by price: ");
                scanf("%s", shopName);
                //二叉树的搜索
                Shop* shop = findShop(currentUser->shop, shopName);
                if (shop == NULL) {
                    printf("Shop not found!\n");
                    break;
                }

                sortProductsByPrice(&shop->products);
                printf("Products sorted by price successfully!\n");
            }
            else {
                printf("Login as a seller to sort products by price!\n");
            }
            break;
        case 10:
            currentUser = NULL;
            printf("Logged out successfully!\n");
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice! Please enter a number between 0-10.\n");
        }
    } while (choice != 0);

    return 0;
}