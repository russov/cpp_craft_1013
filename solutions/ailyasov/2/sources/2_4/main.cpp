#include "trades_filter.h"

int main() {
    TradesFilter::filter_trades_and_save(
            SOURCE_DIR "/tests/2.4_example.in",
            SOURCE_DIR "/tests/2.4.out");
    return 0;
} 
