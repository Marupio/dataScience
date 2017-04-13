namespace ds {


bool PktVals::has(CardVal cv) const {
    return first == cv || second == cv;
}


bool PktVals::pairs() const {
    return first == second;
}


bool PktVals::pairs(CardVal cv) const {
    return first == cv && second == cv;
}


bool PktVals::operator==(const PktVals& pc) const {
    return (
        first == pc.first && second == pc.second
    ) || (
        second == pc.first && first == pc.second
    );
}


bool operator!=(const PktVals& pc) {
    return !(operator==(pc));
}
        

operator VecCardVal() {
    return {first, second};
}


} // End namespace ds
