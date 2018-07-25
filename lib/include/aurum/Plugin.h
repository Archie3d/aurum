#ifndef AURUM_PLUGIN_H
#define AURUM_PLUGIN_H

#include <memory>
#include <map>
#include <string>

namespace Steinberg {
namespace Vst {
    class EditController;
    class AudioEffect;
    class ParameterContainer;
}
}

namespace nana {
    class form;
}

namespace au {
namespace plugin {

/**
 * @brief Parameters container.
 */
class ParameterContainer
{
public:

    /// Parameter accessor helper
    struct Accessor
    {
        ParameterContainer &container;
        int id = 0;
        bool valid = false;

        Accessor(ParameterContainer &c, int id, bool valid)
            : container(c),
              id(id),
              valid(valid)
        {
        }

        Accessor& operator =(double v)
        {
            if (valid) {
                container.setValue(id, v);
            }
            return *this;
        }

        double operator()() const {
            return value();
        }

        double value() const {
            if (valid) {
                return container.value(id);
            }
            return 0.0;
        }
    };

    ParameterContainer();
    ParameterContainer(const ParameterContainer&) = delete;
    ParameterContainer& operator =(const ParameterContainer&) = delete;

    void bind(Steinberg::Vst::EditController *pCtrl, Steinberg::Vst::ParameterContainer *pParameters);

    double value(int id) const;
    void setValue(int id, double v);

    Accessor operator[](int id);

    /**
     * @brief Add a control parameter.
     *
     * @note Currently all the parameters are treated as normilized, and can be automated.
     *
     * @param id
     * @param name
     * @param value
     * @param units
     * @param midiCtrl
     */
    void add(int id, const std::string &name, double value=0.0, const std::string &units=std::string(), int midiCtrl=0);

    /**
     * @brief Find MIDI control number to parameter id mapping if defined.
     * @param midiCtrl
     * @param id
     * @return
     */
    bool findMidiCtrlMap(int midiCtrl, int &id) const;

private:
    struct Impl;
    std::unique_ptr<Impl> d;
};

/**
 * @brief Interface to a plugin controller.
 */
class IController
{
public:
    virtual ~IController() = default;

    /**
     * @brief Initialize controller.
     *
     * This method will be called before the internal VST edit controller initialization.
     * This is a place where control parameters can be created.
     */
    virtual void initialize() = 0;

    /**
     * @brief Finalize controller.
     */
    virtual void finalize() = 0;

    /**
     * @brief Bind this controller with VST edit controller.
     * @param pCtrl
     */
    virtual void bind(Steinberg::Vst::EditController *pCtrl) = 0;

    /**
     * @brief Returns reference to the parameters container.
     * @return
     */
    virtual ParameterContainer& parameters() = 0;
};

/**
 * @brief Abstract controllerimplementation.
 */
class Controller : public IController
{
public:
    Controller();
    ~Controller();
    void initialize() override {}
    void finalize() override {}

    void bind(Steinberg::Vst::EditController *pCtrl) final override;

    ParameterContainer& parameters() override;

private:

    struct Impl;
    std::unique_ptr<Impl> d;
};

/**
 * @brief View (UI) interface.
 */
class IView
{
public:
    virtual ~IView() = default;
};

class View : public IView
{
public:
    View(nana::form &form, IController &controller)
        : m_controller(controller)
    {}

protected:

    IController& controller() { return m_controller; }

private:
    IController &m_controller;
};

struct IMinimumViewSize
{
};

template <int w, int h>
struct MinimumViewSize : public IMinimumViewSize
{
    static constexpr int minimumViewWidth = w;
    static constexpr int minimumViewHeight = h;
};

struct IMaximumViewSize
{
};

template <int w, int h>
struct MaximumViewSize : public IMaximumViewSize
{
    static constexpr int maximumViewWidth = w;
    static constexpr int maximumViewHeight = h;
};

class IAudioEffect
{
public:
    virtual ~IAudioEffect() = default;
    virtual void setActive(bool) = 0;
    virtual bool isActive() const = 0;

    virtual void initialize() = 0;
    virtual void finalize() = 0;

    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void bind(Steinberg::Vst::AudioEffect *pEffect) = 0;

    virtual void setSampleRate(double sr) = 0;
    virtual double sampleRate() const = 0;
};

class IParametersListener
{
public:
    virtual ~IParametersListener() = default;
    virtual void parameterChanged(int id, double value) = 0;
};

class IMidiListener
{
public:
    virtual ~IMidiListener() = default;
    virtual void noteOn(int number, float velocity) = 0;
    virtual void noteOff(int number, float velocity) = 0;
};

class IStereoOutput
{
public:
    virtual ~IStereoOutput() = default;
    virtual void process(float *pLeft, float *pRight, int numSamples) = 0;
};

class AudioEffect : public IAudioEffect
{
public:
    AudioEffect();
    ~AudioEffect();
    void setActive(bool a) override;
    bool isActive() const override;

    void initialize() override {}
    void finalize() override {}
    void start() override {}
    void stop() override {}

    void bind(Steinberg::Vst::AudioEffect *pEffect) final override;

    void setSampleRate(double sr) override;
    double sampleRate() const override;
private:

    struct Impl;
    std::unique_ptr<Impl> d;
};

} // namespace plugin
} // namespace au

#endif // AURUM_PLUGIN_H
