// #include<bits/stdc++.h>  
// long long Quick_Mod(long long a, long long b, long long mod)//快速幂  
// {    
//     long long res = 1,term = a % mod;    
//     while(b)    
//     {    
//         if(b & 1) res = (res * term) % mod;    
//         term = (term * term) % mod;    
//         b >>= 1;    
//     }    
//     return res;    
// }    
// //费马检测  
// bool Is_Prime(long long n)    
// {    
//    int i;    
//    srand(time(0));  
//    for(i = 0;i < 5;i++)//随机次数    
//        if(Quick_Mod(1+rand()%(n-1) ,n - 1,n) != 1)    
//          break;    
//    if(i == 5) return true;    
//    return false;    
// }  