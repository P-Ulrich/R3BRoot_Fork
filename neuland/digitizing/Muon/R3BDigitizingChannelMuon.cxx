// //Test
//
//
// auto QdCToTimeOverThreshold(double qdc){
//
//     fit_module_par_= GetHitModulePar();
//     return (qdc * fit_module_par_.energyGain + fit_module_par_.PMTThreshold);};
// //or like this
//
//
// auto QdCToTimeOverThreshold(double qdc, double energy_gain, double pmt_threshold ){return (qdc * fit_module_par_.energy_gain + fit_module_par_.pmt_threshold);};
// //or without private member valuables??? 
//
//
//
//
//
//
//
//
//
// //creating a flag??
// //
//
//
// #include <iostream>
// #include <cstring> // Für strcmp
//
// void useCorrectionMethods() {
//     std::cout << "Korrekturmethoden werden verwendet." << std::endl;
// }
//
// void regularMethods() {
//     std::cout << "Normale Methoden werden verwendet." << std::endl;
// }
//
// int main(int argc, char *argv[]) {
//     bool useCorrection = false;
//
//     for (int i = 1; i < argc; ++i) {
//         if (strcmp(argv[i], "--use-correction") == 0) {
//             useCorrection = true;
//         }
//     }
//
//     if (useCorrection) {
//         useCorrectionMethods();
//     } else {
//         regularMethods();
//     }
//
//     return 0;
// }
//
// //or runtime
//
// #include <iostream>
//
// void useCorrectionMethods() {
//     std::cout << "Korrekturmethoden werden verwendet." << std::endl;
//     // Ihre Korrekturmethoden-Logik hier
// }
//
// void regularMethods() {
//     std::cout << "Normale Methoden werden verwendet." << std::endl;
//     // Ihre normale Logik hier
// }
//
// int main() {
//     char userInput;
//     std::cout << "Möchten Sie die Korrekturmethoden verwenden? (j/n): ";
//     std::cin >> userInput;
//
//     if (userInput == 'j' || userInput == 'J') {
//         useCorrectionMethods();
//     } else {
//         regularMethods();
//     }
//
//     return 0;
// }
//
// //NEW Channel
//
//
    struct MuonSignal
    {
        double time_over_threshold{};
        double time_leading_edge{};
        ChannelSide side{};
        double bar_id{};
    };

namespace R3B::Digitizing::Neuland
{

auto MuonChannel::ConstructSignals() -> MuonSignals
    {
        fqt_peaks_ = ConstructFQTPeaks(pmt_peaks_);
        // signal pileup:
        FQTPeakPileUp(fqt_peaks_);

        // construct Channel signals:
        auto signals = std::vector<Signal>{};
        signals.reserve(fqt_peaks_.size());

        for (const auto& peak : fqt_peaks_)
        {
            signals.emplace_back(CreateSignal(peak));
        }
        return signals;
    }
}
