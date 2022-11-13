#include <bits/stdc++.h>
using namespace std  ;
 
class bplus ; 

class node {
	friend class bplus ; 
	friend class internal_node ; 
	public : int is , loc = -1 ;
	public : map < int , node* > pnt ;  
	public : set < int > key ; 
	public : node* parent = nullptr ;
	public : virtual node* insert( int , node*)  = 0 ; 
	public : virtual node* search( int ) = 0 ; 
	public : virtual void print( int height ) = 0 ; 
	public : virtual void update_parent( node* ) = 0 ; 
	public : virtual node* find_sibling( node*  ) { return this ; } ; 
	public : virtual void del_value( int val ) { return ; } ; 
	public : virtual void up_key( int val ,  node* chng  ) {  return  ; } ; 
	public : virtual int search_key( node* ) {  return 0 ; } ; 
 }; 

class internal_node : private node {
	friend class bplus  ; 
	friend class leaf_node ; 
	friend class address_node ; 
	static int order ; 
	bplus* belong ; 
	internal_node( int val , node* address1 , node* address2 , bplus* belong ){
		address1->update_parent( this ) ; 
		address2->update_parent( this ) ; 
		pnt[val] = address1 ; 
		pnt[INT_MAX] = address2 ; 
		key.insert( val ) ; 
		key.insert( INT_MAX ) ; 
		this->belong = belong ; 
		this->is = 0 ; this->loc = -1 ; 
		parent = nullptr ; 
	}

	node* insert( int val , node* point ){
		auto it3 = key.lower_bound( val ) ; 
		key.insert( val ) ; 
		pnt[val] = pnt[(*it3)] ; 
		pnt[(*it3)] = point ; 

		if( (key.size()) <= order ){
			node* ret ; ret = this ; 
			while( ret->parent != nullptr ){
				ret = ret -> parent ; 
			}
			return ret ; 
		}

		int y = ( order + 1 ) / 2 ; 
		auto key_replace = key ; 
		auto pnt_replace = pnt ; 
		key.clear() ; pnt.clear() ; 
		int i = 1 ,  up_key ; node* up_pnt ; 

		auto it = key_replace.begin() ;  
		while( i < y ){
			key.insert( (*it) ) ; 
			pnt[(*it)] = pnt_replace[(*it)] ; 
			it++ ; i++ ; 
		}
		key.insert( INT_MAX ) ; 
		pnt[INT_MAX] = pnt_replace[(*it)] ; 
		up_key = (*it) ; 
		it++ ; auto it2 = it ; it++ ; i+=2 ; 

		auto new_node = new internal_node( (*it2) , pnt_replace[(*it2)] , pnt_replace[*(it)] , belong ) ; 

		while( i <=  pnt_replace.size()){
			new_node->key.insert( (*it) ) ;  
			new_node->pnt[(*it)] = pnt_replace[(*it)] ; 
			pnt_replace[(*it)]->update_parent( new_node ) ; 
			i++;
		}
		
		if( parent == nullptr ){
			parent = new internal_node( up_key , this , new_node , belong ) ; 
			new_node-> parent = parent ; 
			return parent ; 
		}else{
			return parent->insert( up_key , new_node ) ; 
		}

	}

	void del_value( int val ){

		if( !pnt.count( val )) {
			return ; 
		}

		node* sib ; 

		int who = 0 ; 

		if( parent != nullptr ){
			sib = parent->find_sibling( this ) ; 
			if( (*(sib->pnt.begin()) ).first < (*(this->pnt.begin())).first   ){
				who =  1 ; 
			}	
		}
		
		this->pnt.erase( val ) ; this->key.erase( val ) ; 
		
		if( pnt.size() >= ( order + 1 ) / 2 ) {
			return  ; 
		}

		if( parent == nullptr ){
			return ; 
		}

		
		
		if( sib->pnt.size() >= ( order + 1 ) / 2 + 1  ){

			if( who ){
				auto it = sib->pnt.end() ; it-- ; 
				this->pnt[(*it).first] = (*it).second ;  
				this->key.insert( (*it).first ) ; 
				sib->pnt.erase( (*it).first ) ; 
				sib->key.erase( (*it).first ) ; 
				(*it).second->update_parent( this ) ; 
				parent->up_key( (*it).first , sib ) ; 
			}else{
				auto it = sib->pnt.begin() ; 
				this->pnt[(*it).first] = (*it).second ;  
				this->key.insert( (*it).first ) ; 
				sib->pnt.erase( (*it).first ) ; 
				sib->key.erase( (*it).first ) ;
				(*it).second -> update_parent( this ) ; 
 				parent->up_key( (*it).first , this ) ; 
			}

			return ; 

		}

		 

			if( who  ){	
				for( auto g : sib->pnt ){
					pnt[g.first] = g.second ; 
					g.second->update_parent( this ) ; 
					key.insert( g.first ) ; 
				}
				parent->del_value( parent->search_key( sib ) ) ; 
			}else{
					
				

				for( auto g : pnt ){
					sib->pnt[g.first] = g.second ; 
					sib->key.insert( g.first ) ; 
					g.second->update_parent( sib ) ; 
				}
				
				parent->del_value( parent->search_key( this ) ) ; 
 			}

	}

	node* search( int val ){
		auto it = key.lower_bound( val ) ; 
		int s_key = (*it) ; 
		return pnt[s_key] ; 
	}

	void print( int height ){
		cout << "internal_"  << height << ' ' ; 
		for( auto g : key ){
			if( g == INT_MAX ){
				break ; 
			}
			cout << g << ' ' ; 
		}
		cout << '\n' ; 
		for( auto g : pnt ){
			g.second -> print( height - 1 ) ; 
		}
	}
	
	void update_parent( node* par  ){
		this->parent =  par ; 
	}

	node* find_sibling( node* curr ){
		int fnd = 0 ; 
		for( auto g : pnt ){
			if( g.second == curr ){
				break ; 
			}
			fnd++ ; 
		}
		auto it = pnt.end() ; 
		it-- ; 
		if( fnd + 1 == pnt.size() ){
			it-- ; 
		}else{
			for( int j = pnt.size( ) - 2  ; j > fnd ; j -- ){
				it-- ; 
			}
		}
		
		return it->second ;  
	}

	int search_key ( node* check ){
		for( auto g : pnt ){
			if( g.second == check ){
				return g.first ; 
			}
		}
		return 0 ; 
	}

	void up_key( int val , node* chng  ){
		int chng_key ; 
		for( auto g : pnt ){
			if( g.second == chng ){
				chng_key = g.first ; 
			}
		}
		pnt.erase( chng_key ) ; 
		pnt[val] = chng ; 
		key.erase( chng_key ) ; 
		key.insert( val ) ; 
	}

}; 

class leaf_node : private node  { 
	friend class bplus  ;
	friend class internal_node ; 
	friend class address_node ;  
	static int order ; 
	bplus* belong ; 
	leaf_node* next ; 
	leaf_node* prev ; 
	leaf_node( bplus* belong ){
		next = nullptr ; 
		prev = nullptr ; 
		parent = nullptr ; 
		this->belong = belong ; 
		this->is = 1 ; this-> loc = -1 ; 
	}

	node* insert( int val , node* reg_address ){
		 
		pnt[val] = reg_address ;
		if( pnt.size() <= order ){ 
			if( this->parent == nullptr ){
				return this ; 
			} 
			node* ret = this->parent ; 
			while( ret -> parent != nullptr ){
				ret = ret -> parent ; 
			}
			return ret ; 
		}
		

		auto new_node = new leaf_node(belong) ; 
		
		new_node->next = next ;  
		next = new_node ; 
		
		int y = ( order + 2 ) / 2 ; 
		map < int , node* > replace = pnt ; 
		pnt.clear() ; 
		auto it = replace.begin() ; 
		int i = 0 , up_key ;  
		
		while( i < y ){
			up_key = it->first ; 
			pnt[it->first] = it->second ; 
			it++ ; i ++ ; 
		}
		while( i < replace.size()){
			new_node->pnt[it->first] = it->second ; 
			it++ ; i ++ ; 
		}
		
		if( parent != nullptr ){
			
			new_node->parent = parent ; 
			return parent->insert( up_key , new_node ) ; 
		}else{
			
			parent = new internal_node( up_key , this , new_node , belong ) ; 
			new_node-> parent = parent ;  
			return parent ; 
		}
	}

	void del_value( int val ){
		if( !pnt.count(val)){
			return ; 
		}
		node* sib   ; 
		int who = 0 ;

		if( parent != nullptr ){
 			sib = parent->find_sibling( this ) ; 
 			
 			if( (*(sib->pnt.begin()) ).first < (*(this->pnt.begin())).first   ){
				who = 1 ; 
			}	
		} 
		
		pnt.erase( val ) ; 

		if( pnt.size() >= ( order + 1 )/2 ){
			return ; 
		}
		if( parent == nullptr ){
			return ; 
		}
		

 
		if( sib->pnt.size() >= ( order + 1 ) / 2 + 1  ){

			if( who  ){
				auto it = sib->pnt.end() ; it-- ; 
				this->pnt[(*it).first] = (*it).second ;  
				sib->pnt.erase( (*it).first ) ; 
				parent->up_key( (*it).first , sib ) ; 
			}else{
				auto it = sib->pnt.begin() ; 
				this->pnt[(*it).first ] = (*it).second ; 
				sib->pnt.erase( (*it).first ) ; 
				parent->up_key( (*it).first , this ) ; 
			}

			return ; 
		}

		

			if( !who  ){	
				for( auto g : sib->pnt ){
					pnt[g.first] = g.second ; 
				}		
				parent->del_value( parent->search_key( sib ) ) ; 
			}else{
				for( auto g : pnt ){
					sib->pnt[g.first] = g.second ; 
				}

				parent->del_value( parent->search_key( this ) ) ; 
 			}		

	}

	node* find_sibling( node* curr ){
		return this ; 
	}

	node* search( int val ){
		if( !pnt.count( val )){
			cout << "Not_found" << '\n' ; 
		}

		return pnt[val] ; 
	}

	void print( int height ){
		cout << "leaf " << ' '  ; 
		for( auto g : pnt ){
			cout << g.first << ' ' ; 
		}
		cout << '\n' ;
	}

	void update_parent( node* par ){ 
		this->parent = par ; 
	}

}; 


class address_node : private node{

	friend class bplus ; 
	friend class leaf_node ; 
	friend class node ; 
	address_node( int loc ){
		this->is = -1 ; 
		this-> loc = loc ; 
	}

	node* insert( int val , node* add ){
		return this ; 
	}

	node* search( int val ){
		return this ; 
	}

	void print(  int height ){
		cout << loc << '\n' ; 
	}

	void update_parent( node* par ){
		this->parent = par ; 
	}


};

class bplus{

	friend class internal_node ; 
	friend class leaf_node ; 
	friend class  node ; 
	friend class address_node ; 
	int level = 0 ; 
	int order_interal_node , order_leaf_node ; 
	node* root ; 
	leaf_node* leaf_node_root ; 
	internal_node* internal_node_root ; 


	public : bplus( 	int order_interal_node,
						int order_leaf_node ) 
						: order_interal_node( order_interal_node ) , order_leaf_node ( order_leaf_node ){
							
			assert( order_interal_node >= 3 ) ; 
			root = new leaf_node( this ) ; 
		 
	}

	public : int search( int val ){

		auto sch = root ; 
		while( !(sch-> is) ){
			sch = sch->search( val ) ; 
		}
		return (sch->search( val ) )-> loc ; 
	}

	node* pvt_search( int val ){
		auto sch = root ; 
		while( !(sch-> is) ){
			sch = sch->search( val ) ; 
		}
		return sch ; 
	}

	public : int insert( int val , int loc ){
		auto address = new address_node( loc ) ; 
		auto sch_node = pvt_search( val ) ; 
		auto y = root ; 
		root = sch_node->insert( val , address ) ;  
		if( y !=  root ){
			level++ ; 
		}
		return 0 ; 
	}

	public : void del_value( int val ){
		auto sch_node = pvt_search( val ) ; 
		sch_node->del_value( val ) ; 
	}

	public : void print( ){
		root->print(level) ; 
	}


}; 

int internal_node::order = 3 ; 
int leaf_node::order = 2 ; 

int main() {

	bplus c( 4 , 5 ) ; 

	c.insert( 11, 10 ); 
	c.insert( 20 , 10 ); 
	c.insert( 14 , 17 );
	c.insert( 25 , 40 ); 
	c.insert( 30 , 40 ); 
	c.insert( 12 , 20 );
	c.insert( 22 , 40 ); 
	c.insert( 23 , 40 ); 
	c.insert( 24 , 40 );
	c.del_value( 20 ) ; 
	c.del_value( 22 ) ; 
    c.del_value( 24 ) ; 
    c.del_value( 14 ) ;  
	c.print()  ; 

}