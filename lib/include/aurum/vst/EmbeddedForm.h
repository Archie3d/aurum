#ifndef AURUM_VST_EMBEDDED_FORM_H
#define AURUM_VST_EMBEDDED_FORM_H

#include <memory>

namespace nana {
    class form;
}

namespace au {
namespace vst {

/**
 * @brief Hosted form.
 *
 * This class defined an embeddable form, which will be
 * placed inside of a parent host window.
 */
class EmbeddedForm final
{
public:
    explicit EmbeddedForm();
    EmbeddedForm(const EmbeddedForm&) = delete;
    EmbeddedForm& operator =(const EmbeddedForm&) = delete;
    ~EmbeddedForm();

    void attachToHost(void *pParentHandle);

    nana::form& form();
    void resize(int width, int height);
    int width() const;
    int height() const;

    int minimumWidth() const;
    int minimumHeight() const;
    void minimumSize(int width, int height);

    int maximumWidth() const;
    int maximumHeight() const;
    void maximumSize(int width, int height);

    void reset();
    void show();
private:

    struct Impl;
    std::unique_ptr<Impl> d;
};

} // namespace vst
} // namespace au

#endif // AURUM_VST_EMBEDDED_FORM_H
