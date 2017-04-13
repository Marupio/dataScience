#ifndef VecValStats_h
#define VecValStats_h

#include<ValStats.h>
#include<PktCards.h>

// Contains meta data about a set of cards, organized by card value.
// Does not contain references to set of cards, and therefore cannot update
// values if these change.  It must be reconstructed.

namespace ds {

// Forward declarations
class Board;

class VecValStats
{
public:

    // Public type data
    
        typedef std::vector<ValStats>::const_iterator const_iterator;
        typedef std::vector<ValStats>::const_reverse_iterator
            const_reverse_iterator;
        typedef std::vector<ValStats>::size_type size_type;
        typedef std::vector<ValStats>::difference_type difference_type;


    // Constructors
    
        //- Construct null
        VecValStats();
    
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

        //- Return number of cards used to create these statistics
        short nStatCards() const {
            return nStatCards_;
        }

        //- Access to front of vector
        const ValStats& front() const {
            return data_.front();
        }

        //- Access to back of vector
        const ValStats& back() const {
            return data_.back();
        }


        // Iterator access
        
            //- const_iterator begin
            const_iterator cbegin() const noexcept {
                return data_.cbegin();
            }

            //- const_iterator end
            const_iterator cend() const noexcept {
                return data_.cend();
            }

            //- const_reverse_iterator begin
            const_reverse_iterator crbegin() const noexcept {
                return data_.crbegin();
            }

            //- const_iterator end
            const_reverse_iterator crend() const noexcept {
                return data_.crend();
            }

            //- Return size
            size_type size() const noexcept {
                return data_.size();
            }


private:
    
    // Private data
    
        //- Number of cards on which these stats are based
        short nStatCards_;

        //- Contains the metadata
        //  Each element contains the following data:
        //  <0> CardVal:     card value
        //  <1> short:       number of cards with this card value
        //  <2> SuitCount:   suits represented
        std::vector<ValStats> data_;


    // Private functions
    
        //- Make meta data
        void makeMetaData(const VecCard& vc);
};

} // end namespace ds

#endif
