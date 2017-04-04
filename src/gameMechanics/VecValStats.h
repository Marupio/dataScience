#include<ValStats.h>

// Contains meta data about a set of cards, organized by card value.
// Does not contain references to set of cards, and therefore cannot update
// values if these change.  It must be reconstructed.

// Forward declarations
class Board;

class VecValStats
{
public:

    // Public type data
    
        typedef std::Vector<ValStats>::const_iterator const_iterator;
        typedef std::Vector<ValStats>::const_reverse_iterator
            const_reverse_iterator;
        typedef std::Vector<ValStats>::size_type size_type;
        typedef std::Vector<ValStats>::difference_type difference_type;


    // Constructors
    
        //- Construct from vector of cards
        VecValStats(const VecCard& cards);

        //- Construct from board
        VecValStats(const Board& bd);
        
        //- Construct from board and pocket
        VecValStats(const Board& bd, const PktCards& pkt);
        
        //- Construct from deck indices
        VecValStats(const VecDeckInd& vdi);


    //- Destructor
    ~VecValStats() {}


    // Public member functions

        // Iterator access
        
            //- const_iterator begin
            const_iterator cbegin() const noexcept {
                return const_iterator(data_.cbegin());
            }

            //- const_iterator end
            const_iterator cend() const noexcept {
                return const_iterator(data_.cend());
            }

            //- const_reverse_iterator begin
            const_iterator crbegin() const noexcept {
                return const_iterator(data_.crbegin());
            }

            //- const_iterator end
            const_iterator crend() const noexcept {
                return const_iterator(data_.crend());
            }

            //- Return size
            size_type size() const noexcept {
                return data_.size();
            }

       
private:
    
    // Private data
    
        //- Number of cards on which these stats are based
        short nCards_;

        //- Contains the metadata
        //  Each element contains the following data:
        //  <0> CardVal:     card value
        //  <1> short:       number of cards with this card value
        //  <2> SuitCount:   suits represented
        std::Vector<ValStats> data_;


    // Private functions
    
        //- Make meta data
        void makeMetaData(const VecCard& vc);
};