#if !defined STRUCTS_HPP
#define STRUCTS_HPP
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cmath>
#include <cstddef>
#include <stdexcept>

// This struct is used to hold the data read from a CSV file. Each member variable is a vector that holds one column of data.
// The headers vector holds the names of the columns, and the other vectors hold the data for each column.
// The rows() function returns the number of rows in the data, which is equal to the size of the timestamp vector (or any other vector, since they should all be the same size).
struct CSVData {
    std::vector<std::string> headers; // column names
    std::vector<std::tm> timestamp;   // "YYYY-MM-DD hh:mm:ss"
    std::vector<unsigned int>    count, day_index; // count of bikes, day index (0=Jan 1, 1=Jan 2, ...)
    std::vector<double> hour_sin, hour_cos, day_sin, day_cos; // sine and cosine transformations of hour and day of week
    std::vector<uint8_t>    is_weekend , is_february, is_holiday; // boolean flags for weekend, February, and holiday

    size_t rows() const { return timestamp.size(); } // Returns the number of rows in the data, which is equal to the size of the timestamp vector.
};

// This function parses a timestamp string in the format "YYYY-MM-DD hh:mm:ss" into a std::tm struct.
inline bool parse_timestamp(const std::string& s, std::tm& out) {
    std::istringstream ss(s); // Create an input string stream from the timestamp string
    ss >> std::get_time(&out, "%Y-%m-%d %H:%M:%S"); // Parse the timestamp string into the std::tm struct using the specified format
    return !ss.fail();
}

// Example use:
// Metrics m(0.5);
// for (size_t i = 0; i < y_true.size(); ++i) m.update(y_true[i], y_pred[i]);
// std::cout << "MSE " << m.mse() << "  MAE " << m.mae() << "  R2 " << m.r2() << '\n';
// This class is used to compute various metrics for regression and classification tasks.
// It keeps track of the number of samples, sum of squared errors, sum of absolute errors, sum of true values, and sum of squared true values for regression metrics.
// For classification metrics, it keeps track of true positives, false positives, false negatives, and true negatives.
// The class provides methods to update the metrics with new samples and to compute various performance metrics such as mean squared error (MSE),
// root mean squared error (RMSE), mean absolute error (MAE), R-squared (R2), precision, recall, F1 score, and accuracy.
class Metrics {
public:
    // Constructor that initializes the threshold for classification metrics.
    explicit Metrics(double threshold = 0.5) : threshold_(threshold) {}

    // Update the metrics with a new sample. y_true is the true value, and y_pred is the predicted value.
    void update(double y_true, double y_pred) {
        const double err = y_true - y_pred;
        ++n_;
        sum_sq_err_  += err * err;
        sum_abs_err_ += std::abs(err);
        sum_y_       += y_true;
        sum_y2_      += y_true * y_true;

        const bool t = y_true >= threshold_;
        const bool p = y_pred >= threshold_;
        if (t && p)        ++tp_;
        else if (!t && p)  ++fp_;
        else if (t && !p)  ++fn_;
        else               ++tn_;
    }

    // Accessor methods for the number of samples and various regression and classification metrics.
    size_t count() const { return n_; }
    double mse() const { return n_ ? sum_sq_err_ / n_ : 0.0; }
    double rmse() const { return std::sqrt(mse()); }
    double mae() const { return n_ ? sum_abs_err_ / n_ : 0.0; }

    double r2() const {
        if (n_ < 2) return 0.0;
        const double ss_tot = sum_y2_ - sum_y_ * sum_y_ / n_;
        return ss_tot > 0.0 ? 1.0 - sum_sq_err_ / ss_tot : 0.0;
    }

    double precision() const { return (tp_ + fp_) ? double(tp_) / (tp_ + fp_) : 0.0; }
    double recall()    const { return (tp_ + fn_) ? double(tp_) / (tp_ + fn_) : 0.0; }
    double f1() const {
        const double p = precision(), r = recall();
        return (p + r) > 0.0 ? 2.0 * p * r / (p + r) : 0.0;
    }
    double accuracy() const { return n_ ? double(tp_ + tn_) / n_ : 0.0; }

    // Accessor methods for true positives, false positives, false negatives, and true negatives.
    size_t tp() const { return tp_; }
    size_t fp() const { return fp_; }
    size_t fn() const { return fn_; }
    size_t tn() const { return tn_; }

private:
    double threshold_; // threshold for classification
    size_t n_ = 0; // number of samples
    // For regression metrics
    double sum_sq_err_ = 0.0, sum_abs_err_ = 0.0, sum_y_ = 0.0, sum_y2_ = 0.0;
    // For classification metrics
    size_t tp_ = 0, fp_ = 0, fn_ = 0, tn_ = 0;
};

// Example use:
// MulticlassMetrics m(3);
//  for (size_t i = 24; i < val.rows(); ++i) {
//      size_t y_true = to_class(val.count[i],      t_low, t_high);
//      size_t y_pred = to_class(val.count[i - 24], t_low, t_high);
//      m.update(y_true, y_pred);
//  }
// std::cout << "Accuracy " << m.accuracy() << "  Macro F1 " << m.macro_f1() << "  Weighted F1 " << m.weighted_f1() << '\n';
// This class is used to compute various metrics for multiclass classification tasks.
// It keeps track of the number of classes and a confusion matrix that counts the number of true positives, false positives, false negatives, and true negatives for each class.
// The class provides methods to update the metrics with new samples and to compute various performance metrics such as accuracy, precision, recall, F1 score, macro F1 score, and weighted F1 score.
class MulticlassMetrics {
public:
    // Constructor that initializes the number of classes and the confusion matrix.
    explicit MulticlassMetrics(size_t num_classes)
        : K_(num_classes), C_(num_classes * num_classes, 0) {
        if (K_ < 2) throw std::invalid_argument("need at least 2 classes");
    }

    // Update the confusion matrix with a new sample. y_true is the true class label, and y_pred is the predicted class label.
    void update(size_t y_true, size_t y_pred) {
        if (y_true >= K_ || y_pred >= K_)
            throw std::out_of_range("class label out of range");
        ++C_[y_true * K_ + y_pred];
        ++n_;
    }

    size_t at(size_t t, size_t p) const { return C_[t * K_ + p]; } // Access the confusion matrix at row t and column p.

    // Accuracy is calculated as the number of correct predictions divided by the total number of predictions.
    double accuracy() const {
        size_t correct = 0;
        for (size_t k = 0; k < K_; ++k) correct += at(k, k);
        return n_ ? double(correct) / n_ : 0.0;
    }

    // Precision is calculated as the number of true positives for a given class divided by the total number of predicted instances of that class.
    double precision(size_t k) const {
        size_t col = 0;
        for (size_t i = 0; i < K_; ++i) col += at(i, k);
        return col ? double(at(k, k)) / col : 0.0;
    }

    // Recall is calculated as the number of true positives for a given class divided by the total number of actual instances of that class.
    double recall(size_t k) const {
        size_t row = 0;
        for (size_t j = 0; j < K_; ++j) row += at(k, j);
        return row ? double(at(k, k)) / row : 0.0;
    }

    // F1 score is calculated as the harmonic mean of precision and recall for a given class.
    double f1(size_t k) const {
        const double p = precision(k), r = recall(k);
        return (p + r) > 0.0 ? 2.0 * p * r / (p + r) : 0.0;
    }

    // Macro F1 score is calculated by taking the F1 score for each class and averaging them.
    double macro_f1() const {
        double s = 0.0;
        for (size_t k = 0; k < K_; ++k) s += f1(k);
        return s / K_;
    }

    // Weighted F1 score is calculated by taking the F1 score for each class and weighting it by the number of true instances for that class.
    double weighted_f1() const {
        double s = 0.0;
        for (size_t k = 0; k < K_; ++k) {
            size_t support = 0;
            for (size_t j = 0; j < K_; ++j) support += at(k, j);
            s += f1(k) * support;
        }
        return n_ ? s / n_ : 0.0;
    }

private:
    size_t K_; // number of classes
    std::vector<size_t> C_;   // row-major K x K
    size_t n_ = 0; // total number of samples
};

// This struct is used to hold an error message and the line number where the error occurred.
// It is used in the validation functions to return an error message and the line number of the first error encountered.
struct ValidationError {
    std::string message;   // empty means success
    size_t row = 0; // line number of the first error encountered

    bool ok() const { return message.empty(); } // Returns true if there is no error message, indicating success.
    static ValidationError success() { return {}; } // Returns a ValidationError object indicating success (no error).
    static ValidationError fail(std::string msg, size_t r) { return {std::move(msg), r}; } // Returns a ValidationError object indicating failure, with the provided error message and line number.
};

#endif // STRUCTS_HPP