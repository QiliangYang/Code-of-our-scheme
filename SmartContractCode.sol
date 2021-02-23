pragma solidity >=0.4.0 <0.7.0;


contract Worker{
    
    uint256[] public Set_attributes;
    uint256   public node_number;
    

    
    function init_node(uint256 index) public returns(uint256){   
        
        Set_attributes   = new uint256[](index);
        node_number = index;
  
        return 0;
    }

    function set_node(uint256 index, uint256 data) public returns(uint256){   
        
        Set_attributes[index] = data;
  
        return 0;
    }
   
    function set_zero_node(uint256 index) public returns(uint256){   
        
        Set_attributes[index] = 0;
  
        return 0;
    }
  
    
}