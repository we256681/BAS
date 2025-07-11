#ifndef SOLVERFACTORY_H
#define SOLVERFACTORY_H
#include "engine_global.h"

#include "isolverfactory.h"
#include "manualcaptchasolver.h"
#include "antigatecaptchasolver.h"
#include "ihttpclientfactory.h"
#include <QMap>
namespace BrowserAutomationStudioFramework
{

    class ENGINESHARED_EXPORT SolverFactory : public ISolverFactory
    {
        Q_OBJECT
        ManualCaptchaSolver * ManualSolver;
        AntigateCaptchaSolver * AntigateSolver;
        AntigateCaptchaSolver * DbcSolver;
        AntigateCaptchaSolver * RucaptchaSolver;
        AntigateCaptchaSolver * TwocaptchaSolver;
        AntigateCaptchaSolver * CapMonsterSolver;
        AntigateCaptchaSolver * XevilSolver;
        IHttpClientFactory * HttpClientFactory;
        QMap<QString, ISolver*> Replaced;
    public:
        explicit SolverFactory(QObject *parent = 0);
        virtual ISolver* GetSolver(const QString& name);
        virtual void ReplaceSolver(const QString& name, ISolver* Solver);

        void SetHttpClientFactory(IHttpClientFactory * HttpClientFactory);
        IHttpClientFactory * GetHttpClientFactory();
    signals:

    private slots:
        void UsedManual();
        void UsedAntigate();
        void UsedDbc();
        void UsedRucaptcha();
        void Used2Captcha();
        void UsedCapmonster();
        void UsedXevil();

        void FailedManual();
        void FailedAntigate();
        void FailedDbc();
        void FailedRucaptcha();
        void Failed2Captcha();
        void FailedCapmonster();
        void FailedXevil();
    };
}

#endif // SOLVERFACTORY_H
