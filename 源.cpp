#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ��Ʒ��Ϣ�ṹ��
typedef struct Product {
    char productId[20];
    char productName[50];
    int price;
    int quantity;
    struct Product* next; // ָ����������һ����Ʒ��ָ��
} Product;
// ���̽ṹ��
typedef struct Shop {
    char shopName[50];
    char ownerName[50];
    Product* products; // �õ��̵���Ʒ����ͷָ��
    struct Shop* left; // ������������ָ��
    struct Shop* right; // ������������ָ��
} Shop;

// �û��ṹ��
typedef struct User {
    char username[50];
    char password[50];
    int userType; // 1: ���ң�2: ���
    Shop* shop; // ��������ң�ָ������̣��������ң�Ϊ��
    struct User* next;
} User;



// ȫ�ֱ�����ָ���û������ͷָ��
User* userList = NULL;

// �Ƚ������������Ƶ�˳���ֵ���ıȽϣ����ں����ĵ��̶���������
//������const char*���ã���Ϊ��ֻ����״̬�������shopname��ɸ���
int compareShopNames(const char* shopName1, const char* shopName2) {
    return strcmp(shopName1, shopName2);
}

// �ݹ�����µ��̵��������У��������Ĳ���
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

// �ݹ��������Ʒ����Ʒ�����У�����Ĳ���
void insertProduct(Product* currentProduct, Product* newProduct) {
    if (currentProduct->next == NULL) {
        currentProduct->next = newProduct;
    }
    else {
        insertProduct(currentProduct->next, newProduct);
    }
}

// �����ض����̵ĺ���������������������Ϊ��ֱ�ӷ���null
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
        return root;//��ǰ�ݹ鵽�Ľ�㣬����ָ���ڵ�
    }
}

// ע�ắ�����û��������붼�����ظ���ѡ���û����ͣ��ɹ��󣬽����µ��û����
//�����û����뵽�û�����
void registerUser() {
    char username[50];
    char password[50];
    int userType;

    // �����û���
    printf("Enter username: ");
    scanf("%s", username);

    // ����û����Ƿ��Ѵ���
    User* currentUser = userList;
    while (currentUser != NULL) {
        if (strcmp(currentUser->username, username) == 0) {
            printf("User %s already registered! Please choose a different username.\n", username);
            return; // ����û����ظ���ֱ�ӷ��أ�������ע��
        }
        currentUser = currentUser->next;
    }

    // ��������
    printf("Enter password: ");
    scanf("%s", password);

    // ��������Ƿ��������û������ظ�
    currentUser = userList;//���´�ͷ��㿪ʼ
    while (currentUser != NULL) {
        if (strcmp(currentUser->password, password) == 0) {
            printf("Password already in use! Please choose a different password.\n");
            return; // ��������ظ���ֱ�ӷ��أ�������ע��
        }
        currentUser = currentUser->next;
    }

    // �����û�����
    printf("Enter user type (1 for seller, 2 for buyer): ");
    scanf("%d", &userType);

    // �������û��ڵ�
    User* newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }//����ʧ��

    // �������û�����Ϣ
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->userType = userType;
    newUser->shop = NULL;

    // �����û��ڵ���뵽�û��б�Ŀ�ͷ(��ͷ���)
    newUser->next = userList;
    userList = newUser;

    printf("Registration successful!\n");
}

// ��¼�������û����������ƥ�䣬����ı��������Ƿ�ƥ��
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

// ��ӵ��̣�����newshop��㣬������Ϣ����ʼ����Ʒ�������������Ϊ��
//���ö������Ĳ��뺯����ʹ�û������ж������
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

    // �����µ��̵����ҵĵ�������
    if (seller->shop == NULL) {
        seller->shop = newShop;//�����ĸ�
    }
    else {
        insertShop(seller->shop, newShop);//����������
    }

    printf("Shop added successfully!\n");
}

// ɾ������(����������������)����Ʒ��������Ƿ�Ϊ�գ��������ݹ�ɾ������������ͬʱ
//ͨ����Ʒ����ı���������Ӧ��Ʒһһɾ��
void deleteShop(Shop* shop) {
    if (shop == NULL) {
        return;
    }

    // �ݹ�ɾ����������
    deleteShop(shop->left);
    deleteShop(shop->right);

    // �ͷŸõ��̵���Ʒ�����ڴ�
    Product* currentProduct = shop->products;
    while (currentProduct != NULL) {
        Product* temp = currentProduct;       //��Ҫ������������ɾ��
        currentProduct = currentProduct->next;
        free(temp);
    }
    shop->products = NULL; //��ȷ��������δ�ͷŵ���Ʒ����ڵ㡣

    // �ͷŵ��̽ṹ���ڴ�
    free(shop);
}

// ɾ�����ҵĵ�����Ϣ��������������޵��̣����������������ҵ���Ӧ�ĵ��̣�
//ɾ��Ŀ����̲��ؽ���������Ȼ������ִ��״̬
void deleteSellerShop(User* seller) {
    if (seller->shop == NULL) {
        printf("You have no shops to delete!\n");
        return;     //�����̸��ڵ��Ƿ����
    }

    char shopName[50];
    printf("Enter shop name to delete: ");
    scanf("%s", shopName);

    Shop* parent = NULL;
    Shop* current = seller->shop;//��Ϊ���ڵ�
    Shop* target = NULL;

    // ����Ҫɾ���ĵ��̼��丸�ڵ�
    while (current != NULL) {
        int cmp = compareShopNames(shopName, current->shopName);
        if (cmp == 0) {
            target = current;
            break;
        }
        parent = current;//����parentΪcurrent
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

    // ɾ��Ŀ����̲��ؽ����������ִ��״̬
    if (parent == NULL) {
        // Ҫɾ�����Ǹ�����
        seller->shop = NULL;
    }
    else if (parent->left == target) {
        parent->left = NULL;
    }
    else {
        parent->right = NULL;
    }

    deleteShop(target);//�����ɾ������������
    printf("Shop %s deleted successfully!\n", shopName);
}


// �����Ʒ�����̵���Ʒ����������޵��̣�Ȼ�������������ҵ���Ӧ�ĵ��̣�
// �ݹ鷽������Ʒ������Ʒ����
void addProduct(User* seller) {
    if (seller->shop == NULL) {
        printf("You need to add a shop first!\n");
        return;
    }

    char shopName[50];
    printf("Enter shop name to add product to: ");
    scanf("%s", shopName);

    Shop* shop = findShop(seller->shop, shopName);//�ҵ���Ӧ����
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

    // ������Ʒ��ӵ����̵���Ʒ������
    if (shop->products == NULL) {
        shop->products = newProduct;
    }
    else {
        insertProduct(shop->products, newProduct);
    }

    printf("Product added successfully!\n");
}

// ɾ��ָ����Ʒ��������̣���Ʒ�Ƿ�Ϊ�գ�����prevָ�룬���������ҵ�����Ʒ
//����ɾ��ͷ�������м仹��β��
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
            // ɾ��ͷ����Ʒ
            shop->products = current->next;
        }
        else {
            // ɾ���м��β����Ʒ
            prev->next = current->next;
        }
        free(current);
        printf("Product %s deleted successfully!\n", productId);
    }
    else {
        printf("Product %s not found!\n", productId);
    }
}

// ͨ����ƷID���Һ���ʾ��Ʒ��Ϣ�������̼���Ʒ�Ƿ�Ϊ�գ������������ҵ���Ӧ���̣�
//���������ͨ��id�ҵ����Ӧ��Ʒ
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

// ������ȱ�����������������ʽ�����������ң�������������
//��������������Ʒ����ı���
void dfsShopTree(Shop* shop) {
    if (shop == NULL) {
        return;
    }
    // ��������������������ٸ��ڵ㣬���������
    dfsShopTree(shop->left);
    printf("Shop Name: %s, Owner: %s\n", shop->shopName, shop->ownerName);
    printf("Products:\n");
    // �������̵���Ʒ����
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

// ���۸�������Ʒ����ͨ��ָ���ָ�����ı�����˳�����˲���������㷨
void sortProductsByPrice(Product** headRef) {
    if (*headRef == NULL || (*headRef)->next == NULL) {
        return;
        //headRef ��һ��ָ�� Product ����ͷָ���ָ�루Product**����
        //*headRef ��һ��ָ�� Product ����ͷ�ڵ��ָ��
        //�������Ϊ�ջ�ֻ��һ���ڵ㣬��ֱ�ӷ��أ���������

    }

    Product* sorted = NULL;
    Product* current = *headRef;

    while (current != NULL) {
        Product* next = current->next;
        //����ǰ�ڵ�֮ǰ������һ���ڵ�
        if (sorted == NULL || sorted->price > current->price) {
            current->next = sorted;
            sorted = current;//����Ĳ���(ͷ��)
        }
        else {
            Product* temp = sorted;
            while (temp->next != NULL && temp->next->price <= current->price) {
                temp = temp->next;
            }
            current->next = temp->next;//����Ĳ���(�м��β��)
            temp->next = current;
        }

        current = next;//����ǰ�ڵ����Ϊ֮ǰ����� next �ڵ㣬
        //�Ա�������������е���һ���ڵ㡣
    }

    *headRef = sorted;
}


//�ļ���¼��
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
        // ���ҵ���
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
        // ���ҵ���
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



// ������
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
        printf("6. Delete product\n");       //��������̱���
        printf("7. Find product by ID\n");   //��������̣�����
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
            currentUser = loginUser();//��Ҫ�и���ǰ�û����ܷ���ֵ
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
        case 6:         //������������
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
                //�������ı���
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
                //������������
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